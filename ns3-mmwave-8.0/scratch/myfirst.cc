/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation;
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "ns3/applications-module.h"
#include "ns3/core-module.h"
#include "ns3/internet-module.h"
#include "ns3/network-module.h"
#include "ns3/point-to-point-module.h"

// Default Network Topology
//
//       10.1.1.0
// n0 -------------- n1
//    point-to-point
//

using namespace ns3;

NS_LOG_COMPONENT_DEFINE("FirstScriptExample");

int
main(int argc, char* argv[])
{
    double port = 80;
    double sizePacket = 26;
    CommandLine cmd(__FILE__);
    cmd.AddValue("port","Configuraçao da porta de comunicação", port);
    cmd.AddValue("size_packet","Numero de pakoes para enviar", sizePacket);
    cmd.Parse(argc, argv);   

    /*O mais baixo numero que pode acontecer entre um intervalo ou evento, em este caso esta configurado como nanosegundo*/ 
    Time::SetResolution(Time::NS);  

    /*Nivel de logs entre INFO, WARNERS, DEBUG */                                     
    LogComponentEnable("UdpEchoClientApplication", LOG_LEVEL_INFO);     
    LogComponentEnable("UdpEchoServerApplication", LOG_LEVEL_INFO);

    

     /*Se criam dois nodos que atuam como computadores, dentro de cada um dos nós irao ter protocolos, 
     endreços  que serão definidos nas proximas linhas*/ 
     NS_LOG_INFO("Creating Topology");
    NodeContainer nodes;           
    nodes.Create(2);        

    /*Criaçao e configuraçao do enlace ponto a ponto que irao conectar os dois nós*/
    PointToPointHelper pointToPoint; 
    
    pointToPoint.SetDeviceAttribute("DataRate", StringValue("5Mbps"));  /*Configuração do trafego entre os dois nós (DateRate)*/
    pointToPoint.SetChannelAttribute("Delay", StringValue("2ms"));  /*Delay*/


    /**************************************************************************************************************/

    /*Criaçao de um contendor com nome devices, onde vão estar alocados os nós creados na linha 49*/
    NetDeviceContainer devices;
    devices = pointToPoint.Install(nodes);

    InternetStackHelper stack;
    stack.Install(nodes);

    /*Configuraçao do endereço ip, para cada um dos nós da rede, em este caso o primeioro nó 
    posue endereço 10.1.1.1 e consecutivamente o nó 2 tem endereço 10.1.1.2*/
    Ipv4AddressHelper address;
    address.SetBase("10.1.1.0", "255.255.255.0");

    /*Atribui o endereçpo base em uma */
    Ipv4InterfaceContainer interfaces = address.Assign(devices);

    UdpEchoServerHelper echoServer(port);

    ApplicationContainer serverApps = echoServer.Install(nodes.Get(1));
    serverApps.Start(Seconds(1.0));
    serverApps.Stop(Seconds(10.0));

    UdpEchoClientHelper echoClient(interfaces.GetAddress(1), port);
    echoClient.SetAttribute("MaxPackets", UintegerValue(5));
    echoClient.SetAttribute("Interval", TimeValue(Seconds(2.0)));
    echoClient.SetAttribute("PacketSize", UintegerValue(sizePacket));

    ApplicationContainer clientApps = echoClient.Install(nodes.Get(0));
    clientApps.Start(Seconds(2.0));
    clientApps.Stop(Seconds(10.0));

    pointToPoint.EnablePcapAll("myfirst");

    Simulator::Run();
    Simulator::Destroy();
    return 0;
}
