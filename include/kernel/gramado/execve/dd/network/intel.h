/*
 * File: nic.h
 *
 * Descrição:
 *     Header para driver de adaptador de rede.
 *     network interface card (NIC)
 *  
 * Opções:
 * Oracle virtual box: // 1022, 2000  //Advanced Micro Devices, 
 * PCnet LANCE PCI Ethernet Controller.
 * ASUS H81: REALTEK, VENDOR 0x10EC; DEVICE 0x8168 
 *
 * 8086/100e network controller encontrado na oracle virtual box.
 *
 * History:
 *     2016 - Created by Fred Nora.
 *      
 */

/*
VirtualBox (3.1 is all I can personally confirm) supports rather dodgy 
implementations of an Intel PRO/1000 MT Server (82545EM), Intel PRO/1000 MT Desktop (82540EM), and Intel PRO/1000 T Server (82543GC).
Bugs:
The EERD register is unimplemented (you *must* use the 4-wire access method 
if you want to read from the EEPROM). [01000101 - I had a patch committed to fix this. It will soon be mainstream]
VMWare Virtual Server 2 emulates/virtualizes an 82545EM-based card rather well.
QEMU (since 0.10.0) supports an 82540EM-based card and it seems to work OK. It is the default network card since 0.11.0.
Bugs:
QEMU does not properly handle the software reset operation (CTRL.RST) in builds prior to June 2009.
*/
 
//NIC 82540EM Intel PRO/1000 MT   8086 100e
 
/*
Description
Hardware Type
The type of the hardware layer over which the ARP packet is being sent. 
Ethernet is 0x1, and you shouldn't need any other value unless you're working with PPP or something.

Protocol Type
The type of the protocol address that the ARP request uses. IP is 0x0800. 
Again, you shouldn't need another value here unless you have unusual requirements 
(in which case you'll hopefully know what you're doing anyway).

Hardware Length
The length of the hardware address for the type of hardware layer. MAC addresses are 6 bytes in length.

Protocol Length
The length of the protocol address for the protocol layer. IPv4 addresses are 4 bytes in length.

Operation Code
The operation to perform.

Source/Dest Hardware Address
Hardware addresses of the source and destination hosts. For an ARP request most implementations 
zero the destination MAC address.

Source/Dest Protocol Address
Protocol addresses of the source and destination hosts
*/

//#define PCI_VENDOR_ID_INTEL	0x8086
//#define PCI_DEVICE_ID_I82540EM	0x100e

//uint8_t broadcast_mac_address[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

// ethernet header
struct ether_header {
	
	uint8_t dst[6];
	uint8_t src[6];
	uint16_t type;
	
} __attribute__((packed)); 


// arp struct
struct  ether_arp {
	
    uint16_t type;  //Hardware type (HTYPE)
	uint16_t proto; //Protocol type (PTYPE)
	uint8_t hlen;   //Hardware address length 
	uint8_t plen;   //Protocol address length 
	uint16_t op;    //Operation (OPER)

   //#define ETH_ALEN 6 
	
	uint8_t arp_sha[6];    /* sender hardware address mac */
    uint8_t arp_spa[4];    /* sender protocol address ip */
	
    uint8_t arp_tha[6];    /* target hardware address mac*/
    uint8_t arp_tpa[4];    /* target protocol address ip*/
	
} __attribute__((packed)); 



//
//  ## TX ##
//

//======================
//legacy transmit descriptor
//  63            48 47   40 39   32 31   24 23   16 15             0
//  +---------------------------------------------------------------+
//  |                         Buffer address                        |
//  +---------------+-------+-------+-------+-------+---------------+
//  |    Special    |  CSS  | Status|  Cmd  |  CSO  |    Length     |
//  +---------------+-------+-------+-------+-------+---------------+

/*

 #obs: Vamos alocar dinamicamente um array com estruturas como essas.
       Cada estrutura vai apontar para um buffer. O endereço do buffer 
	   precisa ser um endereço físico.
*/

#define TDESC_STA_DD   0x01 /* indicates hardware done with descriptor */
#define TDESC_CMD_EOP  0x01 /* indicates end of packet */
#define TDESC_CMD_IFCS 0x02 /* insert frame checksum (FCS) */
#define TDESC_CMD_RS   0x08 /* requests status report */


//intel defines
/*
#define CTRL_SLU	(1 << 6)  //Set Link Up 
#define TCTL_EN		(1 << 1)
#define RCTL_EN		(1 << 1)
#define ICR_TXDW	     (1) //Transmit Descriptor Written Back 
#define ICR_TXQE	(1 << 1) //Transmit Queue Empty 
#define ICR_RXO		(1 << 6) //Receiver Overrun 
#define IMS_RXO		(1 << 6) //Receiver FIFO Overrun 
#define TDESC_EOP	     (1) // End Of Packet 
#define TDESC_RS	(1 << 3) // Report Status 
#define RDESC_STA_DD	     (1) // Descriptor Done 
#define TDESC_STA_DD	     (1) // Descriptor Done 
*/
	   
struct legacy_tx_desc 
{	
	//uint64_t addr;
	uint32_t addr;  //endereço físico do buffer.
	uint32_t addr2;
	
	uint16_t length;
	uint8_t cso;    /* checksum offset */
	uint8_t cmd;
	
	uint8_t status;   /* status and reserved */
	uint8_t css;    /* checksum start */
	uint16_t special;
};

//==========
/* Transmit Descriptor */   
//esse tipo não é o legado.
/*
struct e1000_tx_desc {
	
    //uint64_t buffer_addr;       // Address of the descriptor's data buffer 
    uint32_t buffer_addr1;
	uint32_t buffer_addr2;
	
	union {
        uint32_t data;
        struct {
            uint16_t length;    // Data buffer length ???
            uint8_t cso;        // Checksum offset 
            uint8_t cmd;        // Descriptor control 
        } flags;
    } lower;
    
	union {
        uint32_t data;
        struct {
            uint8_t status;     // Descriptor status 
            uint8_t css;        // Checksum start
            uint16_t special;
        } fields;
    } upper;
};
*/
//===============


//
//  ## RX ##
//


/* Receive Descriptor */

struct legacy_rx_desc 
{	
    //uint64_t buffer_addr; // Address of the descriptor's data buffer 
    
	uint32_t addr;
	uint32_t addr2;
	
	uint16_t length;     // Length of data DMAed into data buffer 
    uint16_t csum;       // Packet checksum 
    uint8_t status;      // Descriptor status 
    uint8_t errors;      // Descriptor Errors 
    uint16_t special;
	
};



//
// ## device info ##
//

//todo: essa rotina pode ficar em network.h 
//e qualquer dispositivo nic pode usr, mesmo que não seja intel.
struct nic_info_d
{
	object_type_t objectType;
	object_class_t objectClass;
	
	//validação da estrutura.
	//se as flags estiverem acionadas, devemos
	//procurar as informações na estrutura de dispositivo pci.
	
	int used;
	int magic;
	
	//pci device.
    struct pci_device_d *pci;	
	
	//salvando o endereço base.
	unsigned long registers_base_address;
	
	unsigned long DeviceControl;
	
	unsigned long DeviceStatus;
	
	//unsigned char mac0;
	//unsigned char mac1;
	//unsigned char mac2;
	//unsigned char mac3;
	//unsigned char mac4;
	//unsigned char mac5;
	
	uint8_t mac_address[6];
	uint8_t ip_address[4];
	
	//============
	//teste
	
	uint16_t rx_cur;
	uint16_t tx_cur;
	
	int eeprom;
	int use_io;
	
	uint16_t io_base;
	uint32_t mem_base;
	
	struct legacy_rx_desc *legacy_rx_descs; //rx ring virtual address
	uint32_t rx_descs_virt[32];	

	struct legacy_tx_desc *legacy_tx_descs; //tx ring virtual address
	uint32_t tx_descs_virt[8];
	
	uint32_t rx_descs_phys;  //rx ring physical address
	uint32_t tx_descs_phys;	 //tx ring physical address
	
	
	//uint8_t  mac_address[6];
	
	
	//unsigned char bus;
	//unsigned char dev;
	//unsigned char fun;	
	
	
	//unsigned short vendor;
	//unsigned short device;

	// rede.
	//struct network_info_d *network;
	
	//struct device_d     *device;

    //struct nic_info_d *next;	
};
struct nic_info_d *currentNIC;
//...





 
//lista de placas de rede.
//#todo: o valor máximo precisa ser definido. 
unsigned long nicList[8]; 



int e1000_interrupt_flag;

//
// ## prototypes ##
//

//void E1000Handler();

void 
PCIRegisterIRQHandler ( uint16_t bus, 
                        uint8_t slot, 
						uint8_t func, 
						unsigned long handler,   //#bugbug modificamos o tipo. 
						void *priv ); //estrutura

 

int e1000_init_nic ( unsigned char bus, unsigned char dev, unsigned char fun );
//int e1000_init_nic ();

void e1000_setup_irq();
int e1000_reset_controller(); 


//void show_current_nic_info ();

/*
void NetSendEthPacket ( PNetworkDevice dev, 
                        uint8_t dest[6], 
						uint16_t type, 
						uint32_t len, 
						uint8_t *buf );
*/				
		
void testSend();
void E1000Send ( void *ndev, uint32_t len, uint8_t *data);		



void E1000WriteCommand ( struct nic_info_d *d, uint16_t addr, uint32_t val );
uint32_t E1000ReadCommand(struct nic_info_d *d, uint16_t addr) ;	

uint32_t E1000AllocCont ( uint32_t amount, uint32_t *virt );						

					
void nic_i8254x_transmit();

//eeprom
uint32_t E1000ReadEEPROM ( struct nic_info_d *d, uint8_t addr );


void xxxe1000handler();



//
// End.
//

