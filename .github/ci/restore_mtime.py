#!/usr/bin/env python3
"""Set each tracked file's mtime to its last-commit time.

A fresh CI checkout stamps every file 'now', which defeats MSBuild's
up-to-date check and forces a full rebuild even on a build-tree cache hit.
Restoring commit-based mtimes makes unchanged files look older than the
cached .obj so only files changed since the cache recompile.

Dependency-free (stdlib + git), single pass over history, and never fatal:
any failure just skips the optimization and the build proceeds normally.
"""
import os, re, subprocess, sys

STAMP = os.path.join("build", ".ci_built_sha")

def changed_since_cache():
    """Files that differ between the commit the cached build tree was made from
    and HEAD. These keep their fresh 'now' mtime so MSBuild recompiles them.
    Without this, a merge brings in files whose commit time is OLDER than the
    cached .obj files and they were silently skipped (stale exe/dll).
    Returns None when we can't tell - then no mtimes are touched (full rebuild)."""
    try:
        sha = open(STAMP).read().strip()
    except OSError:
        print("restore_mtime: no record of what the cache was built from - full rebuild")
        return None
    if not re.fullmatch(r"[0-9a-f]{40}", sha):
        return None
    try:
        out = subprocess.run(["git", "-c", "core.quotePath=false", "diff", "--name-only", "--no-renames", sha, "HEAD"],
                             capture_output=True, text=True, check=True).stdout
    except Exception as e:
        print(f"restore_mtime: cached commit {sha[:7]} not found ({e}) - full rebuild")
        return None
    files = set(l for l in out.splitlines() if l)
    print(f"restore_mtime: cache built from {sha[:7]}, {len(files)} files changed since - those recompile")
    return files

def main():
    changed = changed_since_cache()
    if changed is None:
        return
    try:
        out = subprocess.run(
            ["git", "-c", "core.quotePath=false", "log",
             "--pretty=format:@@@%ct@@@", "--name-only", "--no-renames"],
            capture_output=True, text=True, check=True).stdout
    except Exception as e:
        print(f"restore_mtime: skipped (git log failed: {e})")
        return

    stamp = re.compile(r"^@@@(\d+)@@@$")
    ts = None
    seen = set()
    n = 0
    for line in out.splitlines():
        if not line:
            continue
        m = stamp.match(line)
        if m:
            ts = int(m.group(1))
            continue
        path = line
        if ts is None or path in seen:
            continue
        seen.add(path)
        if path in changed:
            continue
        if os.path.isfile(path):
            try:
                os.utime(path, (ts, ts))
                n += 1
            except OSError:
                pass
    print(f"restore_mtime: set mtime on {n} files")

if __name__ == "__main__":
    try:
        main()
    except Exception as e:
        print(f"restore_mtime: skipped ({e})")
    sys.exit(0)  # optimization only - never fail the build
