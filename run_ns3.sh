#!/bin/sh

echo "Rodando o simulador NS3"

#export NS_LOG=UdpEchoClientApplication=level_all
#export 'NS_LOG=UdpEchoClientApplication=level_all|prefix_func'
#export 'NS_LOG=*=level_all|prefix_func|prefix_time'

export NS_LOG=""


dir="NS-3_source_code/ns-3.41"
command="NS-3_source_code/ns-3.41/ns3"

if [ $1 = "first" ]; then 
    export NS_LOG=FirstScriptExample=info
    #./$command run "scratch/myfirst --PrintHelp"
    ./$command run scratch/myfirst -- --port=125 --size_packet=64 #> log.out 2>&1
    echo "----------    $1-0.0  ------------------"
    tcpdump -nn -tt -r $dir"/myfirst-0-0.pcap"
    echo "----------    $1-1.0  ------------------"
    tcpdump -nn -tt -r $dir"/myfirst-1-0.pcap"
elif [ $1 = "second" ]; then
    #./$command run "scratch/mysecond --PrintHelp"
    ./$command run scratch/mysecond -- --nCsma=5 --verbose=true
    echo "----------    $1-0.0  ------------------"
    tcpdump -nn -tt -r $dir"/second-0-0.pcap"
    echo "----------    $1-1.0  ------------------"
    tcpdump -nn -tt -r $dir"/second-1-0.pcap"
    echo "----------    $1-2.0  ------------------"
    tcpdump -nn -tt -r $dir"/second-2-0.pcap"
elif [ $1 = "gnu" ]; then
    ./$command run scratch/my_gnuplot
fi