Running

Starting

If you have installed NFD from source code, it is recommended to start NFD with the nfd-start script:

nfd-start
On macOS it may ask for your keychain password or ask nfd wants to sign using key in your keychain. Enter your keychain password and click “Always Allow”.

Later, you can stop NFD with nfd-stop or by simply killing the nfd process.

If you have installed NFD using a package manager, you can start and stop NFD using the operating system’s service manager (such as systemd or launchd) or using “Automatically start NFD” option in NDN Control Center app.

Connecting to remote NFDs

To create a UDP tunnel to a remote NFD, execute the following command in terminal:

nfdc face create udp://<other host>
where <other host> is the name or IP address of the other host (e.g., udp://spurs.cs.ucla.edu). This outputs:

face-created id=308 local=udp4://10.0.2.15:6363 remote=udp4://131.179.196.46:6363 persistency=persistent
To add a route /ndn toward the remote NFD, execute the following command in terminal:

nfdc route add /ndn udp://<other host>
This outputs:

route-add-accepted prefix=/ndn nexthop=308 origin=static cost=0 flags=child-inherit expires=never
The /ndn means that NFD will forward all Interests that start with /ndn through the face to the other host. If you only want to forward Interests with a certain prefix, use it instead of /ndn. This only forwards Interests to the other host, but there is no “back route” for the other host to forward Interests to you. For that, you can rely on automatic prefix propagation feature of NFD or go to the other host and use nfdc to add the route.
