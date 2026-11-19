# Builds Maps\<zone>.poi for the in-game minimap / world map.
# line format: T|x|z|y|name   T = Q quest giver, S store, B bank, P portal (y = height, picks the floor)
import json, os, re, sys, collections
src = os.path.dirname(os.path.abspath(__file__))
out = os.path.join(src, 'Maps')
npcs = json.load(open(os.path.join(src, 'npcs.json')))
ports = json.load(open(os.path.join(src, 'portals.json')))
zones = collections.defaultdict(list)
def zone(w): return os.path.splitext(w)[0].upper()
def clean(s): return re.sub(r'[\r\n|]', ' ', s).strip()
def near(lst, t, x, z, d, name=None):
    for e in lst:
        if e[0] == t and (name is None or e[3] == name) and (e[1]-x)**2 + (e[2]-z)**2 < d*d:
            return e
    return None
for n in npcs:
    if not n.get('enabled', True): continue
    name = clean(n.get('name', ''))
    if not name: continue
    t = {'store': 'S', 'bank': 'B', 'quest': 'Q'}.get(n['type'])
    if not t: continue
    if t == 'Q' and not n.get('quests'): continue
    L = zones[zone(n['world'])]
    e0 = near(L, t, n['x'], n['z'], 100, name)
    if e0 and abs(e0[4] - n.get('y', 0.0)) < 40: continue
    L.append([t, n['x'], n['z'], name, n.get('y', 0.0)])
for p in ports:
    if not p.get('Enabled', True): continue
    x = (p['Min'][0] + p['Max'][0]) / 2; z = (p['Min'][2] + p['Max'][2]) / 2
    base = clean(re.sub(r'\s*#\d+$', '', p['Name']))
    tgt = os.path.splitext(p['TargetWorld'])[0]
    L = zones[zone(p['World'])]
    e = near(L, 'P', x, z, 200)
    if e and abs(e[5] - ((p['Min'][1] + p['Max'][1]) / 2 - 35.0)) > 40: e = None
    if e:
        if tgt not in e[4]: e[4].append(tgt)
        continue
    L.append(['P', x, z, base, [tgt], (p['Min'][1] + p['Max'][1]) / 2 - 35.0])
os.makedirs(out, exist_ok=True)
for zn, L in zones.items():
    with open(os.path.join(out, zn + '.poi'), 'w', newline='\r\n') as f:
        for e in L:
            name = e[3] if e[0] != 'P' else '%s -> %s' % (e[3], ', '.join(e[4]))
            y = e[4] if e[0] != 'P' else e[5]
            f.write('%s|%.1f|%.1f|%.1f|%s\n' % (e[0], e[1], e[2], y, name))
    print(zn, collections.Counter(e[0] for e in L))
