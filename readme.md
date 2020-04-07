# NFD Overview

NDN Forwarding Daemon (NFD) is a network forwarder that implements and evolves together with the Named Data Networking (NDN) protocol. After the initial release, NFD will become a core component of the NDN Platform and will follow the same release cycle.

NFD is developed by a community effort. Although the first release was mostly done by the members of NSF-sponsored NDN project team, it already contains significant contributions from people outside the project team (for more details, refer to AUTHORS.md). We strongly encourage participation from all interested parties, since broader community support is key for NDN to succeed as a new Internet architecture. Bug reports and feedback are highly appreciated and can be made through Redmine site and the ndn-interest mailing list.

The main design goal of NFD is to support diverse experimentation of NDN technology. The design emphasizes modularity and extensibility to allow easy experiments with new protocol features, algorithms, and applications. We have not fully optimized the code for performance. The intention is that performance optimizations are one type of experiments that developers can conduct by trying out different data structures and different algorithms; over time, better implementations may emerge within the same design framework. To facilitate such experimentation with the forwarder, the NFD team has also written a developer’s guide, which details the current implementation and provides tips for extending all aspects of NFD.

NFD will keep evolving in three aspects: improvement of the modularity framework, keeping up with the NDN protocol spec, and addition of other new features. We hope to keep the modular framework stable and lean, allowing researchers to implement and experiment with various features, some of which may eventually work into the protocol spec.

The design and development of NFD benefited from our earlier experience with CCNx software package. However, NFD is not in any part derived from CCNx codebase and does not maintain compatibility with CCNx.

# Major Modules of NFD

NFD has the following major modules:

Core
Provides various common services shared between different NFD modules. These include hash computation routines, DNS resolver, config file, face monitoring, and several other modules.

Faces
Implements the NDN face abstraction on top of different lower level transport mechanisms.

Tables
Implements the Content Store (CS), the Pending Interest Table (PIT), the Forwarding Information Base (FIB), and other data structures to support forwarding of NDN Data and Interest packets.

Forwarding
Implements basic packet processing pathways, which interact with Faces, Tables, and Strategies.

Strategy Support, a major part of the forwarding module
Implements a framework to support different forwarding strategies. It includes StrategyChoice, Measurements, Strategies, and hooks in the forwarding pipelines. The StrategyChoice records the choice of the strategy for a namespace, and Measurement records are used by strategies to store past performance results for namespaces.
Management
Implements the NFD Management Protocol, which allows applications to configure NFD and set/query NFD’s internal states. Protocol interaction is done via NDN’s Interest/Data exchange between applications and NFD.

RIB Management
Manages the routing information base (RIB). The RIB may be updated by different parties in different ways, including various routing protocols, application’s prefix registrations, and command-line manipulation by sysadmins. The RIB management module processes all these requests to generate a consistent forwarding table, and then syncs it up with the NFD’s FIB, which contains only the minimal information needed for forwarding decisions. Strictly speaking RIB management is part of the NFD management module. However, due to its importance to the overall operations and its more complex processing, we make it a separate module.

# Running

## Starting

If you have installed NFD from source code, it is recommended to start NFD with the nfd-start script:

nfd-start

Later, you can stop NFD with nfd-stop or by simply killing the nfd process.

If you have installed NFD using a package manager, you can start and stop NFD using the operating system’s service manager (such as systemd or launchd) or using “Automatically start NFD” option in NDN Control Center app.

## Connecting to remote NFDs

To create a UDP tunnel to a remote NFD, execute the following command in terminal:

nfdc face create udp://<other host>
where <other host> is the name or IP address of the other host (e.g., udp://spurs.cs.ucla.edu). This outputs:

face-created id=308 local=udp4://10.0.2.15:6363 remote=udp4://131.179.196.46:6363 persistency=persistent
To add a route /ndn toward the remote NFD, execute the following command in terminal:

nfdc route add /ndn udp://<other host>  
This outputs:

route-add-accepted prefix=/ndn nexthop=308 origin=static cost=0 flags=child-inherit expires=never
The /ndn means that NFD will forward all Interests that start with /ndn through the face to the other host. If you only want to forward Interests with a certain prefix, use it instead of /ndn. This only forwards Interests to the other host, but there is no “back route” for the other host to forward Interests to you. For that, you can rely on automatic prefix propagation feature of NFD or go to the other host and use nfdc to add the route.
