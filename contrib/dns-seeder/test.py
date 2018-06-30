import socket

seeders = [
    'bwkseed.mempool.pw',
    'bwkseed1.tusccrypto.com',
    'bwkseed2.tusccrypto.com',
    'bwkseed3.tusccrypto.com',
    'bwkseed4.tusccrypto.com',
    'bwkseed5.tusccrypto.com',
    'bwkseed1.tusccrypto.site',
    'bwkseed2.tusccrypto.site',
    'bwkseed3.tusccrypto.site',
    'bwkseed4.tusccrypto.site',
    'bwkseed5.tusccrypto.site'
]

for seeder in seeders:
    try:
        ais = socket.getaddrinfo(seeder, 0)
    except socket.gaierror:
        ais = []
    
    # Prevent duplicates, need to update to check
    # for ports, can have multiple nodes on 1 ip.
    addrs = []
    for a in ais:
        addr = a[4][0]
        if addrs.count(addr) == 0:
            addrs.append(addr)
    
    print(seeder + ' = ' + str(len(addrs)))
