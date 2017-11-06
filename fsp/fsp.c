/*
 * fsp.c
 * 
 * Copyright (C) 2017, Federal University of Santa Catarina.
 * 
 * This file is part of FloripaSat-FSP.
 * 
 * FloripaSat-FSP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FloripaSat-FSP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public License
 * along with FloripaSat-FSP. If not, see <http://www.gnu.org/licenses/>.
 * 
 */

/**
 * \file fsp.c
 * 
 * \brief FloripaSat Protocol library implementation.
 * 
 * \author Gabriel Mariano Marcelino <gabriel.mm8@gmail.com>
 * 
 * \version 1.0-dev
 * 
 * \date 06/11/2017
 * 
 * \addtogroup fsp
 * \{
 */

#include "fsp.h"

uint8_t fsp_my_adr = FSP_ADR_UNKNOWN;

void fsp_init(uint8_t module_adr)
{
    fsp_my_adr = module_adr;
}

void fsp_gen_data_pkt(uint8_t *data, uint8_t data_len, uint8_t dst_adr, bool ack, FSPPacket *fsp)
{
    if (ack)
    {
        fsp_gen_pkt(data, data_len, dst_adr, FSP_PKT_TYPE_DATA_WITH_ACK, fsp);
    }
    else
    {
        fsp_gen_pkt(data, data_len, dst_adr, FSP_PKT_TYPE_DATA, fsp);
    }
}

void fsp_gen_cmd_pkt(uint8_t cmd, uint8_t dst_adr, bool ack, FSPPacket *fsp)
{
    if (ack)
    {
        fsp_gen_pkt(cmd, 1, dst_adr, FSP_PKT_TYPE_CMD_WITH_ACK, fsp);
    }
    else
    {
        fsp_gen_pkt(cmd, 1, dst_adr, FSP_PKT_TYPE_CMD, fsp);
    }
}

void fsp_gen_pkt(uint8_t *payload, uint8_t payload_len, uint8_t dst_adr, uint8_t type, FSPPacket *fsp)
{
    fsp->sod        = FSP_PKT_SOD;
    fsp->src_adr    = fsp_my_adr;
    fsp->dst_adr    = dst_adr;
    fsp->length     = payload_len;
    fsp->type       = type;
    
    uint8_t i = 0;
    for(i=0; i<payload_len; i++)
    {
        fsp->payload[i] = payload[i];
    }
    
    //fsp->crc16      = crc16-ccitt();
}

void fsp_encode(FSPPacket *fsp, uint8_t *pkt, uint8_t *pkt_len)
{
    uint8_t i = 0;
    
    pkt[i++] = fsp->sod;
    pkt[i++] = fsp->src_adr;
    pkt[i++] = fsp->dst_adr;
    pkt[i++] = fsp->length;
    pkt[i++] = fsp->type;
    
    for(j=0; j<fsp->length; j++)
    {
        pkt[i++] = fsp->payload[j];
    }
    
    pkt[i++] = fsp->crc16;
}

void fsp_decode(uint8_t *pkt, uint8_t pkt_len, FSPPacket *fsp)
{
    uint8_t i = 0;
    
    fsp->sod        = pkt[i++];
    fsp->src_adr    = pkt[i++];
    fsp->dst_src    = pkt[i++];
    fsp->length     = pkt[i++];
    fsp->type       = pkt[i++];
    
    uint8_t j = 0;
    for(j=0; j<fsp->length; j++)
    {
        fsp->payload[j] = pkt[i++];
    }
    
    fsp->crc16      = (uint16_t)(pkt[i++]) << 8;
    fsp->crc16      |= (uint16_t)(pkt[i++]);
}

//! \} End of fsp group
