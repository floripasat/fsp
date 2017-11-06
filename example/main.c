#include <fsp/fsp.h>

int main()
{
    fsp_init(FSP_ADR_OBDH);
    
    FSPPacket fsp;
    
    fsp_gen_data_pkt();
    
    fsp_encode();
    
    return 0;
}
