import socket

seeders = [
    'bwkseed.mempool.pw',
<<<<<<< HEAD
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
=======
    'bwkseed1.bulwarkcrypto.com',
    'bwkseed2.bulwarkcrypto.com',
    'bwkseed3.bulwarkcrypto.com',
    'bwkseed4.bulwarkcrypto.com',
    'bwkseed5.bulwarkcrypto.com',
    'bwkseed1.bulwarkcrypto.site',
    'bwkseed2.bulwarkcrypto.site',
    'bwkseed3.bulwarkcrypto.site',
    'bwkseed4.bulwarkcrypto.site',
    'bwkseed5.bulwarkcrypto.site'
>>>>>>> 577e63a28f8333aa64a1eba58ed1eadf58744b97
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
