# Endless Ages Gold - source code

The game client (DirectX 11 version of the Aura engine), server programs and tools for Endless Ages Gold,
the revival of the MMOFPS Endless Ages. Released by Mike (blasterdude9) for anyone to use.

Download it, change it, run your own server, do whatever you want with it.

## Notes
- Builds with Visual Studio 2022 (see `.github/workflows/build.yml` for how the projects are built).
- The client loads the normal Endless Ages game files (EARevolution.APX / .EDB are in `EARevolution.7z`).
- All database logins and server addresses were removed. Put your own in where you see
  `YOUR_SQL_SERVER`, `YOUR_SQL_USER`, `YOUR_SQL_PASSWORD`. The launcher's default server is `127.0.0.1:25858`
  (`EXCore_Platform/EALauncher/EALauncher.cpp`) - change it to your own server.
- The 3ds Max SDK folders (`maxsdk`, `maxsdk8`) are Autodesk's and are not included; install them from Autodesk
  if you need the old 3ds Max exporter plugins.

No support is given. Have fun with it.
