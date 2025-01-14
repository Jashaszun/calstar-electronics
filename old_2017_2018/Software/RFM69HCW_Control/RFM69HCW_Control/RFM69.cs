﻿using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;



namespace RFM69HCW_Control
{
    class RFM69
    {
        #region RFM69registers
        // **********************************************************************************
        // Registers used in driver definition for HopeRF RFM69W/RFM69HW, Semtech SX1231/1231H
        // **********************************************************************************
        // Copyright Felix Rusu (2015), felix@lowpowerlab.com
        // http://lowpowerlab.com/
        // **********************************************************************************
        // License
        // **********************************************************************************
        // This program is free software; you can redistribute it 
        // and/or modify it under the terms of the GNU General    
        // Public License as published by the Free Software       
        // Foundation; either version 2 of the License, or        
        // (at your option) any later version.                    
        //                                                        
        // This program is distributed in the hope that it will   
        // be useful, but WITHOUT ANY WARRANTY; without even the  
        // implied warranty of MERCHANTABILITY or FITNESS FOR A   
        // PARTICULAR PURPOSE.  See the GNU General Public        
        // License for more details.                              
        //                                                        
        // You should have received a copy of the GNU General    
        // Public License along with this program; if not, write 
        // to the Free Software Foundation, Inc.,                
        // 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
        //                                                        
        // Licence can be viewed at                               
        // http://www.fsf.org/licenses/gpl.txt                    
        //
        // Please maintain this license information along with authorship
        // and copyright notices in any redistribution of this code
        // **********************************************************************************
        // RFM69/SX1231 Internal registers addresses
        //**************************************************
        const int REG_FIFO = 0x00;
        const int REG_OPMODE = 0x01;
        const int REG_DATAMODUL = 0x02;
        const int REG_BITRATEMSB = 0x03;
        const int REG_BITRATELSB = 0x04;
        const int REG_FDEVMSB = 0x05;
        const int REG_FDEVLSB = 0x06;
        const int REG_FRFMSB = 0x07;
        const int REG_FRFMID = 0x08;
        const int REG_FRFLSB = 0x09;
        const int REG_OSC1 = 0x0A;
        const int REG_AFCCTRL = 0x0B;
        const int REG_LOWBAT = 0x0C;
        const int REG_LISTEN1 = 0x0D;
        const int REG_LISTEN2 = 0x0E;
        const int REG_LISTEN3 = 0x0F;
        const int REG_VERSION = 0x10;
        const int REG_PALEVEL = 0x11;
        const int REG_PARAMP = 0x12;
        const int REG_OCP = 0x13;
        const int REG_AGCREF = 0x14;  // not present on RFM69/SX1231
        const int REG_AGCTHRESH1 = 0x15;  // not present on RFM69/SX1231
        const int REG_AGCTHRESH2 = 0x16;  // not present on RFM69/SX1231
        const int REG_AGCTHRESH3 = 0x17;  // not present on RFM69/SX1231
        const int REG_LNA = 0x18;
        const int REG_RXBW = 0x19;
        const int REG_AFCBW = 0x1A;
        const int REG_OOKPEAK = 0x1B;
        const int REG_OOKAVG = 0x1C;
        const int REG_OOKFIX = 0x1D;
        const int REG_AFCFEI = 0x1E;
        const int REG_AFCMSB = 0x1F;
        const int REG_AFCLSB = 0x20;
        const int REG_FEIMSB = 0x21;
        const int REG_FEILSB = 0x22;
        const int REG_RSSICONFIG = 0x23;
        const int REG_RSSIVALUE = 0x24;
        const int REG_DIOMAPPING1 = 0x25;
        const int REG_DIOMAPPING2 = 0x26;
        const int REG_IRQFLAGS1 = 0x27;
        const int REG_IRQFLAGS2 = 0x28;
        const int REG_RSSITHRESH = 0x29;
        const int REG_RXTIMEOUT1 = 0x2A;
        const int REG_RXTIMEOUT2 = 0x2B;
        const int REG_PREAMBLEMSB = 0x2C;
        const int REG_PREAMBLELSB = 0x2D;
        const int REG_SYNCCONFIG = 0x2E;
        const int REG_SYNCVALUE1 = 0x2F;
        const int REG_SYNCVALUE2 = 0x30;
        const int REG_SYNCVALUE3 = 0x31;
        const int REG_SYNCVALUE4 = 0x32;
        const int REG_SYNCVALUE5 = 0x33;
        const int REG_SYNCVALUE6 = 0x34;
        const int REG_SYNCVALUE7 = 0x35;
        const int REG_SYNCVALUE8 = 0x36;
        const int REG_PACKETCONFIG1 = 0x37;
        const int REG_PAYLOADLENGTH = 0x38;
        const int REG_NODEADRS = 0x39;
        const int REG_BROADCASTADRS = 0x3A;
        const int REG_AUTOMODES = 0x3B;
        const int REG_FIFOTHRESH = 0x3C;
        const int REG_PACKETCONFIG2 = 0x3D;
        const int REG_AESKEY1 = 0x3E;
        const int REG_AESKEY2 = 0x3F;
        const int REG_AESKEY3 = 0x40;
        const int REG_AESKEY4 = 0x41;
        const int REG_AESKEY5 = 0x42;
        const int REG_AESKEY6 = 0x43;
        const int REG_AESKEY7 = 0x44;
        const int REG_AESKEY8 = 0x45;
        const int REG_AESKEY9 = 0x46;
        const int REG_AESKEY10 = 0x47;
        const int REG_AESKEY11 = 0x48;
        const int REG_AESKEY12 = 0x49;
        const int REG_AESKEY13 = 0x4A;
        const int REG_AESKEY14 = 0x4B;
        const int REG_AESKEY15 = 0x4C;
        const int REG_AESKEY16 = 0x4D;
        const int REG_TEMP1 = 0x4E;
        const int REG_TEMP2 = 0x4F;
        const int REG_TESTLNA = 0x58;
        const int REG_TESTPA1 = 0x5A;  // only present on RFM69HW/SX1231H
        const int REG_TESTPA2 = 0x5C;  // only present on RFM69HW/SX1231H
        const int REG_TESTDAGC = 0x6F;

        //******************************************************
        // RF69/SX1231 bit control definition
        //******************************************************

        // RegOpMode
        const int RF_OPMODE_SEQUENCER_OFF = 0x80;
        const int RF_OPMODE_SEQUENCER_ON = 0x00;  // Default

        const int RF_OPMODE_LISTEN_ON = 0x40;
        const int RF_OPMODE_LISTEN_OFF = 0x00;  // Default

        const int RF_OPMODE_LISTENABORT = 0x20;

        const int RF_OPMODE_SLEEP = 0x00;
        const int RF_OPMODE_STANDBY = 0x04;  // Default
        const int RF_OPMODE_SYNTHESIZER = 0x08;
        const int RF_OPMODE_TRANSMITTER = 0x0C;
        const int RF_OPMODE_RECEIVER = 0x10;


        // RegDataModul
        const int RF_DATAMODUL_DATAMODE_PACKET = 0x00;  // Default
        const int RF_DATAMODUL_DATAMODE_CONTINUOUS = 0x40;
        const int RF_DATAMODUL_DATAMODE_CONTINUOUSNOBSYNC = 0x60;

        const int RF_DATAMODUL_MODULATIONTYPE_FSK = 0x00;  // Default
        const int RF_DATAMODUL_MODULATIONTYPE_OOK = 0x08;

        const int RF_DATAMODUL_MODULATIONSHAPING_00 = 0x00;  // Default
        const int RF_DATAMODUL_MODULATIONSHAPING_01 = 0x01;
        const int RF_DATAMODUL_MODULATIONSHAPING_10 = 0x02;
        const int RF_DATAMODUL_MODULATIONSHAPING_11 = 0x03;


        // RegBitRate (bits/sec) example bit rates
        const int RF_BITRATEMSB_1200 = 0x68;
        const int RF_BITRATELSB_1200 = 0x2B;
        const int RF_BITRATEMSB_2400 = 0x34;
        const int RF_BITRATELSB_2400 = 0x15;
        const int RF_BITRATEMSB_4800 = 0x1A;  // Default
        const int RF_BITRATELSB_4800 = 0x0B;  // Default
        const int RF_BITRATEMSB_9600 = 0x0D;
        const int RF_BITRATELSB_9600 = 0x05;
        const int RF_BITRATEMSB_19200 = 0x06;
        const int RF_BITRATELSB_19200 = 0x83;
        const int RF_BITRATEMSB_38400 = 0x03;
        const int RF_BITRATELSB_38400 = 0x41;

        const int RF_BITRATEMSB_38323 = 0x03;
        const int RF_BITRATELSB_38323 = 0x43;

        const int RF_BITRATEMSB_34482 = 0x03;
        const int RF_BITRATELSB_34482 = 0xA0;

        const int RF_BITRATEMSB_76800 = 0x01;
        const int RF_BITRATELSB_76800 = 0xA1;
        const int RF_BITRATEMSB_153600 = 0x00;
        const int RF_BITRATELSB_153600 = 0xD0;
        const int RF_BITRATEMSB_57600 = 0x02;
        const int RF_BITRATELSB_57600 = 0x2C;
        const int RF_BITRATEMSB_115200 = 0x01;
        const int RF_BITRATELSB_115200 = 0x16;
        const int RF_BITRATEMSB_12500 = 0x0A;
        const int RF_BITRATELSB_12500 = 0x00;
        const int RF_BITRATEMSB_25000 = 0x05;
        const int RF_BITRATELSB_25000 = 0x00;
        const int RF_BITRATEMSB_50000 = 0x02;
        const int RF_BITRATELSB_50000 = 0x80;
        const int RF_BITRATEMSB_100000 = 0x01;
        const int RF_BITRATELSB_100000 = 0x40;
        const int RF_BITRATEMSB_150000 = 0x00;
        const int RF_BITRATELSB_150000 = 0xD5;
        const int RF_BITRATEMSB_200000 = 0x00;
        const int RF_BITRATELSB_200000 = 0xA0;
        const int RF_BITRATEMSB_250000 = 0x00;
        const int RF_BITRATELSB_250000 = 0x80;
        const int RF_BITRATEMSB_300000 = 0x00;
        const int RF_BITRATELSB_300000 = 0x6B;
        const int RF_BITRATEMSB_32768 = 0x03;
        const int RF_BITRATELSB_32768 = 0xD1;
        // custom bit rates
        const int RF_BITRATEMSB_55555 = 0x02;
        const int RF_BITRATELSB_55555 = 0x40;
        const int RF_BITRATEMSB_200KBPS = 0x00;
        const int RF_BITRATELSB_200KBPS = 0xa0;


        // RegFdev - frequency deviation (Hz)
        const int RF_FDEVMSB_2000 = 0x00;
        const int RF_FDEVLSB_2000 = 0x21;
        const int RF_FDEVMSB_5000 = 0x00;  // Default
        const int RF_FDEVLSB_5000 = 0x52;  // Default
        const int RF_FDEVMSB_7500 = 0x00;
        const int RF_FDEVLSB_7500 = 0x7B;
        const int RF_FDEVMSB_10000 = 0x00;
        const int RF_FDEVLSB_10000 = 0xA4;
        const int RF_FDEVMSB_15000 = 0x00;
        const int RF_FDEVLSB_15000 = 0xF6;
        const int RF_FDEVMSB_20000 = 0x01;
        const int RF_FDEVLSB_20000 = 0x48;
        const int RF_FDEVMSB_25000 = 0x01;
        const int RF_FDEVLSB_25000 = 0x9A;
        const int RF_FDEVMSB_30000 = 0x01;
        const int RF_FDEVLSB_30000 = 0xEC;
        const int RF_FDEVMSB_35000 = 0x02;
        const int RF_FDEVLSB_35000 = 0x3D;
        const int RF_FDEVMSB_40000 = 0x02;
        const int RF_FDEVLSB_40000 = 0x8F;
        const int RF_FDEVMSB_45000 = 0x02;
        const int RF_FDEVLSB_45000 = 0xE1;
        const int RF_FDEVMSB_50000 = 0x03;
        const int RF_FDEVLSB_50000 = 0x33;
        const int RF_FDEVMSB_55000 = 0x03;
        const int RF_FDEVLSB_55000 = 0x85;
        const int RF_FDEVMSB_60000 = 0x03;
        const int RF_FDEVLSB_60000 = 0xD7;
        const int RF_FDEVMSB_65000 = 0x04;
        const int RF_FDEVLSB_65000 = 0x29;
        const int RF_FDEVMSB_70000 = 0x04;
        const int RF_FDEVLSB_70000 = 0x7B;
        const int RF_FDEVMSB_75000 = 0x04;
        const int RF_FDEVLSB_75000 = 0xCD;
        const int RF_FDEVMSB_80000 = 0x05;
        const int RF_FDEVLSB_80000 = 0x1F;
        const int RF_FDEVMSB_85000 = 0x05;
        const int RF_FDEVLSB_85000 = 0x71;
        const int RF_FDEVMSB_90000 = 0x05;
        const int RF_FDEVLSB_90000 = 0xC3;
        const int RF_FDEVMSB_95000 = 0x06;
        const int RF_FDEVLSB_95000 = 0x14;
        const int RF_FDEVMSB_100000 = 0x06;
        const int RF_FDEVLSB_100000 = 0x66;
        const int RF_FDEVMSB_110000 = 0x07;
        const int RF_FDEVLSB_110000 = 0x0A;
        const int RF_FDEVMSB_120000 = 0x07;
        const int RF_FDEVLSB_120000 = 0xAE;
        const int RF_FDEVMSB_130000 = 0x08;
        const int RF_FDEVLSB_130000 = 0x52;
        const int RF_FDEVMSB_140000 = 0x08;
        const int RF_FDEVLSB_140000 = 0xF6;
        const int RF_FDEVMSB_150000 = 0x09;
        const int RF_FDEVLSB_150000 = 0x9A;
        const int RF_FDEVMSB_160000 = 0x0A;
        const int RF_FDEVLSB_160000 = 0x3D;
        const int RF_FDEVMSB_170000 = 0x0A;
        const int RF_FDEVLSB_170000 = 0xE1;
        const int RF_FDEVMSB_180000 = 0x0B;
        const int RF_FDEVLSB_180000 = 0x85;
        const int RF_FDEVMSB_190000 = 0x0C;
        const int RF_FDEVLSB_190000 = 0x29;
        const int RF_FDEVMSB_200000 = 0x0C;
        const int RF_FDEVLSB_200000 = 0xCD;
        const int RF_FDEVMSB_210000 = 0x0D;
        const int RF_FDEVLSB_210000 = 0x71;
        const int RF_FDEVMSB_220000 = 0x0E;
        const int RF_FDEVLSB_220000 = 0x14;
        const int RF_FDEVMSB_230000 = 0x0E;
        const int RF_FDEVLSB_230000 = 0xB8;
        const int RF_FDEVMSB_240000 = 0x0F;
        const int RF_FDEVLSB_240000 = 0x5C;
        const int RF_FDEVMSB_250000 = 0x10;
        const int RF_FDEVLSB_250000 = 0x00;
        const int RF_FDEVMSB_260000 = 0x10;
        const int RF_FDEVLSB_260000 = 0xA4;
        const int RF_FDEVMSB_270000 = 0x11;
        const int RF_FDEVLSB_270000 = 0x48;
        const int RF_FDEVMSB_280000 = 0x11;
        const int RF_FDEVLSB_280000 = 0xEC;
        const int RF_FDEVMSB_290000 = 0x12;
        const int RF_FDEVLSB_290000 = 0x8F;
        const int RF_FDEVMSB_300000 = 0x13;
        const int RF_FDEVLSB_300000 = 0x33;


        // RegFrf (MHz) - carrier frequency
        // 315Mhz band
        const int RF_FRFMSB_314 = 0x4E;
        const int RF_FRFMID_314 = 0x80;
        const int RF_FRFLSB_314 = 0x00;
        const int RF_FRFMSB_315 = 0x4E;
        const int RF_FRFMID_315 = 0xC0;
        const int RF_FRFLSB_315 = 0x00;
        const int RF_FRFMSB_316 = 0x4F;
        const int RF_FRFMID_316 = 0x00;
        const int RF_FRFLSB_316 = 0x00;
        // 433mhz band
        const int RF_FRFMSB_433 = 0x6C;
        const int RF_FRFMID_433 = 0x40;
        const int RF_FRFLSB_433 = 0x00;
        const int RF_FRFMSB_434 = 0x6C;
        const int RF_FRFMID_434 = 0x80;
        const int RF_FRFLSB_434 = 0x00;
        const int RF_FRFMSB_435 = 0x6C;
        const int RF_FRFMID_435 = 0xC0;
        const int RF_FRFLSB_435 = 0x00;
        // 868Mhz band
        const int RF_FRFMSB_863 = 0xD7;
        const int RF_FRFMID_863 = 0xC0;
        const int RF_FRFLSB_863 = 0x00;
        const int RF_FRFMSB_864 = 0xD8;
        const int RF_FRFMID_864 = 0x00;
        const int RF_FRFLSB_864 = 0x00;
        const int RF_FRFMSB_865 = 0xD8;
        const int RF_FRFMID_865 = 0x40;
        const int RF_FRFLSB_865 = 0x00;
        const int RF_FRFMSB_866 = 0xD8;
        const int RF_FRFMID_866 = 0x80;
        const int RF_FRFLSB_866 = 0x00;
        const int RF_FRFMSB_867 = 0xD8;
        const int RF_FRFMID_867 = 0xC0;
        const int RF_FRFLSB_867 = 0x00;
        const int RF_FRFMSB_868 = 0xD9;
        const int RF_FRFMID_868 = 0x00;
        const int RF_FRFLSB_868 = 0x00;
        const int RF_FRFMSB_869 = 0xD9;
        const int RF_FRFMID_869 = 0x40;
        const int RF_FRFLSB_869 = 0x00;
        const int RF_FRFMSB_870 = 0xD9;
        const int RF_FRFMID_870 = 0x80;
        const int RF_FRFLSB_870 = 0x00;
        // 915Mhz band
        const int RF_FRFMSB_902 = 0xE1;
        const int RF_FRFMID_902 = 0x80;
        const int RF_FRFLSB_902 = 0x00;
        const int RF_FRFMSB_903 = 0xE1;
        const int RF_FRFMID_903 = 0xC0;
        const int RF_FRFLSB_903 = 0x00;
        const int RF_FRFMSB_904 = 0xE2;
        const int RF_FRFMID_904 = 0x00;
        const int RF_FRFLSB_904 = 0x00;
        const int RF_FRFMSB_905 = 0xE2;
        const int RF_FRFMID_905 = 0x40;
        const int RF_FRFLSB_905 = 0x00;
        const int RF_FRFMSB_906 = 0xE2;
        const int RF_FRFMID_906 = 0x80;
        const int RF_FRFLSB_906 = 0x00;
        const int RF_FRFMSB_907 = 0xE2;
        const int RF_FRFMID_907 = 0xC0;
        const int RF_FRFLSB_907 = 0x00;
        const int RF_FRFMSB_908 = 0xE3;
        const int RF_FRFMID_908 = 0x00;
        const int RF_FRFLSB_908 = 0x00;
        const int RF_FRFMSB_909 = 0xE3;
        const int RF_FRFMID_909 = 0x40;
        const int RF_FRFLSB_909 = 0x00;
        const int RF_FRFMSB_910 = 0xE3;
        const int RF_FRFMID_910 = 0x80;
        const int RF_FRFLSB_910 = 0x00;
        const int RF_FRFMSB_911 = 0xE3;
        const int RF_FRFMID_911 = 0xC0;
        const int RF_FRFLSB_911 = 0x00;
        const int RF_FRFMSB_912 = 0xE4;
        const int RF_FRFMID_912 = 0x00;
        const int RF_FRFLSB_912 = 0x00;
        const int RF_FRFMSB_913 = 0xE4;
        const int RF_FRFMID_913 = 0x40;
        const int RF_FRFLSB_913 = 0x00;
        const int RF_FRFMSB_914 = 0xE4;
        const int RF_FRFMID_914 = 0x80;
        const int RF_FRFLSB_914 = 0x00;
        const int RF_FRFMSB_915 = 0xE4;  // Default
        const int RF_FRFMID_915 = 0xC0;  // Default
        const int RF_FRFLSB_915 = 0x00;  // Default
        const int RF_FRFMSB_916 = 0xE5;
        const int RF_FRFMID_916 = 0x00;
        const int RF_FRFLSB_916 = 0x00;
        const int RF_FRFMSB_917 = 0xE5;
        const int RF_FRFMID_917 = 0x40;
        const int RF_FRFLSB_917 = 0x00;
        const int RF_FRFMSB_918 = 0xE5;
        const int RF_FRFMID_918 = 0x80;
        const int RF_FRFLSB_918 = 0x00;
        const int RF_FRFMSB_919 = 0xE5;
        const int RF_FRFMID_919 = 0xC0;
        const int RF_FRFLSB_919 = 0x00;
        const int RF_FRFMSB_920 = 0xE6;
        const int RF_FRFMID_920 = 0x00;
        const int RF_FRFLSB_920 = 0x00;
        const int RF_FRFMSB_921 = 0xE6;
        const int RF_FRFMID_921 = 0x40;
        const int RF_FRFLSB_921 = 0x00;
        const int RF_FRFMSB_922 = 0xE6;
        const int RF_FRFMID_922 = 0x80;
        const int RF_FRFLSB_922 = 0x00;
        const int RF_FRFMSB_923 = 0xE6;
        const int RF_FRFMID_923 = 0xC0;
        const int RF_FRFLSB_923 = 0x00;
        const int RF_FRFMSB_924 = 0xE7;
        const int RF_FRFMID_924 = 0x00;
        const int RF_FRFLSB_924 = 0x00;
        const int RF_FRFMSB_925 = 0xE7;
        const int RF_FRFMID_925 = 0x40;
        const int RF_FRFLSB_925 = 0x00;
        const int RF_FRFMSB_926 = 0xE7;
        const int RF_FRFMID_926 = 0x80;
        const int RF_FRFLSB_926 = 0x00;
        const int RF_FRFMSB_927 = 0xE7;
        const int RF_FRFMID_927 = 0xC0;
        const int RF_FRFLSB_927 = 0x00;
        const int RF_FRFMSB_928 = 0xE8;
        const int RF_FRFMID_928 = 0x00;
        const int RF_FRFLSB_928 = 0x00;


        // RegOsc1
        const int RF_OSC1_RCCAL_START = 0x80;
        const int RF_OSC1_RCCAL_DONE = 0x40;


        // RegAfcCtrl
        const int RF_AFCCTRL_LOWBETA_OFF = 0x00;  // Default
        const int RF_AFCCTRL_LOWBETA_ON = 0x20;


        // RegLowBat
        const int RF_LOWBAT_MONITOR = 0x10;
        const int RF_LOWBAT_ON = 0x08;
        const int RF_LOWBAT_OFF = 0x00;  // Default

        const int RF_LOWBAT_TRIM_1695 = 0x00;
        const int RF_LOWBAT_TRIM_1764 = 0x01;
        const int RF_LOWBAT_TRIM_1835 = 0x02;  // Default
        const int RF_LOWBAT_TRIM_1905 = 0x03;
        const int RF_LOWBAT_TRIM_1976 = 0x04;
        const int RF_LOWBAT_TRIM_2045 = 0x05;
        const int RF_LOWBAT_TRIM_2116 = 0x06;
        const int RF_LOWBAT_TRIM_2185 = 0x07;


        // RegListen1
        const int RF_LISTEN1_RESOL_64 = 0x50;
        const int RF_LISTEN1_RESOL_4100 = 0xA0;  // Default
        const int RF_LISTEN1_RESOL_262000 = 0xF0;

        const int RF_LISTEN1_RESOL_IDLE_64 = 0x40;
        const int RF_LISTEN1_RESOL_IDLE_4100 = 0x80;  // Default
        const int RF_LISTEN1_RESOL_IDLE_262000 = 0xC0;

        const int RF_LISTEN1_RESOL_RX_64 = 0x10;
        const int RF_LISTEN1_RESOL_RX_4100 = 0x20;  // Default
        const int RF_LISTEN1_RESOL_RX_262000 = 0x30;

        const int RF_LISTEN1_CRITERIA_RSSI = 0x00;  // Default
        const int RF_LISTEN1_CRITERIA_RSSIANDSYNC = 0x08;

        const int RF_LISTEN1_END_00 = 0x00;
        const int RF_LISTEN1_END_01 = 0x02;  // Default
        const int RF_LISTEN1_END_10 = 0x04;


        // RegListen2
        const int RF_LISTEN2_COEFIDLE_VALUE = 0xF5; // Default


        // RegListen3
        const int RF_LISTEN3_COEFRX_VALUE = 0x20; // Default


        // RegVersion
        const int RF_VERSION_VER = 0x24;  // Default


        // RegPaLevel
        const int RF_PALEVEL_PA0_ON = 0x80;  // Default
        const int RF_PALEVEL_PA0_OFF = 0x00;
        const int RF_PALEVEL_PA1_ON = 0x40;
        const int RF_PALEVEL_PA1_OFF = 0x00;  // Default
        const int RF_PALEVEL_PA2_ON = 0x20;
        const int RF_PALEVEL_PA2_OFF = 0x00;  // Default

        const int RF_PALEVEL_OUTPUTPOWER_00000 = 0x00;
        const int RF_PALEVEL_OUTPUTPOWER_00001 = 0x01;
        const int RF_PALEVEL_OUTPUTPOWER_00010 = 0x02;
        const int RF_PALEVEL_OUTPUTPOWER_00011 = 0x03;
        const int RF_PALEVEL_OUTPUTPOWER_00100 = 0x04;
        const int RF_PALEVEL_OUTPUTPOWER_00101 = 0x05;
        const int RF_PALEVEL_OUTPUTPOWER_00110 = 0x06;
        const int RF_PALEVEL_OUTPUTPOWER_00111 = 0x07;
        const int RF_PALEVEL_OUTPUTPOWER_01000 = 0x08;
        const int RF_PALEVEL_OUTPUTPOWER_01001 = 0x09;
        const int RF_PALEVEL_OUTPUTPOWER_01010 = 0x0A;
        const int RF_PALEVEL_OUTPUTPOWER_01011 = 0x0B;
        const int RF_PALEVEL_OUTPUTPOWER_01100 = 0x0C;
        const int RF_PALEVEL_OUTPUTPOWER_01101 = 0x0D;
        const int RF_PALEVEL_OUTPUTPOWER_01110 = 0x0E;
        const int RF_PALEVEL_OUTPUTPOWER_01111 = 0x0F;
        const int RF_PALEVEL_OUTPUTPOWER_10000 = 0x10;
        const int RF_PALEVEL_OUTPUTPOWER_10001 = 0x11;
        const int RF_PALEVEL_OUTPUTPOWER_10010 = 0x12;
        const int RF_PALEVEL_OUTPUTPOWER_10011 = 0x13;
        const int RF_PALEVEL_OUTPUTPOWER_10100 = 0x14;
        const int RF_PALEVEL_OUTPUTPOWER_10101 = 0x15;
        const int RF_PALEVEL_OUTPUTPOWER_10110 = 0x16;
        const int RF_PALEVEL_OUTPUTPOWER_10111 = 0x17;
        const int RF_PALEVEL_OUTPUTPOWER_11000 = 0x18;
        const int RF_PALEVEL_OUTPUTPOWER_11001 = 0x19;
        const int RF_PALEVEL_OUTPUTPOWER_11010 = 0x1A;
        const int RF_PALEVEL_OUTPUTPOWER_11011 = 0x1B;
        const int RF_PALEVEL_OUTPUTPOWER_11100 = 0x1C;
        const int RF_PALEVEL_OUTPUTPOWER_11101 = 0x1D;
        const int RF_PALEVEL_OUTPUTPOWER_11110 = 0x1E;
        const int RF_PALEVEL_OUTPUTPOWER_11111 = 0x1F;  // Default


        // RegPaRamp
        const int RF_PARAMP_3400 = 0x00;
        const int RF_PARAMP_2000 = 0x01;
        const int RF_PARAMP_1000 = 0x02;
        const int RF_PARAMP_500 = 0x03;
        const int RF_PARAMP_250 = 0x04;
        const int RF_PARAMP_125 = 0x05;
        const int RF_PARAMP_100 = 0x06;
        const int RF_PARAMP_62 = 0x07;
        const int RF_PARAMP_50 = 0x08;
        const int RF_PARAMP_40 = 0x09;  // Default
        const int RF_PARAMP_31 = 0x0A;
        const int RF_PARAMP_25 = 0x0B;
        const int RF_PARAMP_20 = 0x0C;
        const int RF_PARAMP_15 = 0x0D;
        const int RF_PARAMP_12 = 0x0E;
        const int RF_PARAMP_10 = 0x0F;


        // RegOcp
        const int RF_OCP_OFF = 0x0F;
        const int RF_OCP_ON = 0x1A;  // Default

        const int RF_OCP_TRIM_45 = 0x00;
        const int RF_OCP_TRIM_50 = 0x01;
        const int RF_OCP_TRIM_55 = 0x02;
        const int RF_OCP_TRIM_60 = 0x03;
        const int RF_OCP_TRIM_65 = 0x04;
        const int RF_OCP_TRIM_70 = 0x05;
        const int RF_OCP_TRIM_75 = 0x06;
        const int RF_OCP_TRIM_80 = 0x07;
        const int RF_OCP_TRIM_85 = 0x08;
        const int RF_OCP_TRIM_90 = 0x09;
        const int RF_OCP_TRIM_95 = 0x0A;  // Default
        const int RF_OCP_TRIM_100 = 0x0B;
        const int RF_OCP_TRIM_105 = 0x0C;
        const int RF_OCP_TRIM_110 = 0x0D;
        const int RF_OCP_TRIM_115 = 0x0E;
        const int RF_OCP_TRIM_120 = 0x0F;


        // RegAgcRef - not present on RFM69/SX1231
        const int RF_AGCREF_AUTO_ON = 0x40;  // Default
        const int RF_AGCREF_AUTO_OFF = 0x00;

        const int RF_AGCREF_LEVEL_MINUS80 = 0x00;  // Default
        const int RF_AGCREF_LEVEL_MINUS81 = 0x01;
        const int RF_AGCREF_LEVEL_MINUS82 = 0x02;
        const int RF_AGCREF_LEVEL_MINUS83 = 0x03;
        const int RF_AGCREF_LEVEL_MINUS84 = 0x04;
        const int RF_AGCREF_LEVEL_MINUS85 = 0x05;
        const int RF_AGCREF_LEVEL_MINUS86 = 0x06;
        const int RF_AGCREF_LEVEL_MINUS87 = 0x07;
        const int RF_AGCREF_LEVEL_MINUS88 = 0x08;
        const int RF_AGCREF_LEVEL_MINUS89 = 0x09;
        const int RF_AGCREF_LEVEL_MINUS90 = 0x0A;
        const int RF_AGCREF_LEVEL_MINUS91 = 0x0B;
        const int RF_AGCREF_LEVEL_MINUS92 = 0x0C;
        const int RF_AGCREF_LEVEL_MINUS93 = 0x0D;
        const int RF_AGCREF_LEVEL_MINUS94 = 0x0E;
        const int RF_AGCREF_LEVEL_MINUS95 = 0x0F;
        const int RF_AGCREF_LEVEL_MINUS96 = 0x10;
        const int RF_AGCREF_LEVEL_MINUS97 = 0x11;
        const int RF_AGCREF_LEVEL_MINUS98 = 0x12;
        const int RF_AGCREF_LEVEL_MINUS99 = 0x13;
        const int RF_AGCREF_LEVEL_MINUS100 = 0x14;
        const int RF_AGCREF_LEVEL_MINUS101 = 0x15;
        const int RF_AGCREF_LEVEL_MINUS102 = 0x16;
        const int RF_AGCREF_LEVEL_MINUS103 = 0x17;
        const int RF_AGCREF_LEVEL_MINUS104 = 0x18;
        const int RF_AGCREF_LEVEL_MINUS105 = 0x19;
        const int RF_AGCREF_LEVEL_MINUS106 = 0x1A;
        const int RF_AGCREF_LEVEL_MINUS107 = 0x1B;
        const int RF_AGCREF_LEVEL_MINUS108 = 0x1C;
        const int RF_AGCREF_LEVEL_MINUS109 = 0x1D;
        const int RF_AGCREF_LEVEL_MINUS110 = 0x1E;
        const int RF_AGCREF_LEVEL_MINUS111 = 0x1F;
        const int RF_AGCREF_LEVEL_MINUS112 = 0x20;
        const int RF_AGCREF_LEVEL_MINUS113 = 0x21;
        const int RF_AGCREF_LEVEL_MINUS114 = 0x22;
        const int RF_AGCREF_LEVEL_MINUS115 = 0x23;
        const int RF_AGCREF_LEVEL_MINUS116 = 0x24;
        const int RF_AGCREF_LEVEL_MINUS117 = 0x25;
        const int RF_AGCREF_LEVEL_MINUS118 = 0x26;
        const int RF_AGCREF_LEVEL_MINUS119 = 0x27;
        const int RF_AGCREF_LEVEL_MINUS120 = 0x28;
        const int RF_AGCREF_LEVEL_MINUS121 = 0x29;
        const int RF_AGCREF_LEVEL_MINUS122 = 0x2A;
        const int RF_AGCREF_LEVEL_MINUS123 = 0x2B;
        const int RF_AGCREF_LEVEL_MINUS124 = 0x2C;
        const int RF_AGCREF_LEVEL_MINUS125 = 0x2D;
        const int RF_AGCREF_LEVEL_MINUS126 = 0x2E;
        const int RF_AGCREF_LEVEL_MINUS127 = 0x2F;
        const int RF_AGCREF_LEVEL_MINUS128 = 0x30;
        const int RF_AGCREF_LEVEL_MINUS129 = 0x31;
        const int RF_AGCREF_LEVEL_MINUS130 = 0x32;
        const int RF_AGCREF_LEVEL_MINUS131 = 0x33;
        const int RF_AGCREF_LEVEL_MINUS132 = 0x34;
        const int RF_AGCREF_LEVEL_MINUS133 = 0x35;
        const int RF_AGCREF_LEVEL_MINUS134 = 0x36;
        const int RF_AGCREF_LEVEL_MINUS135 = 0x37;
        const int RF_AGCREF_LEVEL_MINUS136 = 0x38;
        const int RF_AGCREF_LEVEL_MINUS137 = 0x39;
        const int RF_AGCREF_LEVEL_MINUS138 = 0x3A;
        const int RF_AGCREF_LEVEL_MINUS139 = 0x3B;
        const int RF_AGCREF_LEVEL_MINUS140 = 0x3C;
        const int RF_AGCREF_LEVEL_MINUS141 = 0x3D;
        const int RF_AGCREF_LEVEL_MINUS142 = 0x3E;
        const int RF_AGCREF_LEVEL_MINUS143 = 0x3F;


        // RegAgcThresh1 - not present on RFM69/SX1231
        const int RF_AGCTHRESH1_SNRMARGIN_000 = 0x00;
        const int RF_AGCTHRESH1_SNRMARGIN_001 = 0x20;
        const int RF_AGCTHRESH1_SNRMARGIN_010 = 0x40;
        const int RF_AGCTHRESH1_SNRMARGIN_011 = 0x60;
        const int RF_AGCTHRESH1_SNRMARGIN_100 = 0x80;
        const int RF_AGCTHRESH1_SNRMARGIN_101 = 0xA0;  // Default
        const int RF_AGCTHRESH1_SNRMARGIN_110 = 0xC0;
        const int RF_AGCTHRESH1_SNRMARGIN_111 = 0xE0;

        const int RF_AGCTHRESH1_STEP1_0 = 0x00;
        const int RF_AGCTHRESH1_STEP1_1 = 0x01;
        const int RF_AGCTHRESH1_STEP1_2 = 0x02;
        const int RF_AGCTHRESH1_STEP1_3 = 0x03;
        const int RF_AGCTHRESH1_STEP1_4 = 0x04;
        const int RF_AGCTHRESH1_STEP1_5 = 0x05;
        const int RF_AGCTHRESH1_STEP1_6 = 0x06;
        const int RF_AGCTHRESH1_STEP1_7 = 0x07;
        const int RF_AGCTHRESH1_STEP1_8 = 0x08;
        const int RF_AGCTHRESH1_STEP1_9 = 0x09;
        const int RF_AGCTHRESH1_STEP1_10 = 0x0A;
        const int RF_AGCTHRESH1_STEP1_11 = 0x0B;
        const int RF_AGCTHRESH1_STEP1_12 = 0x0C;
        const int RF_AGCTHRESH1_STEP1_13 = 0x0D;
        const int RF_AGCTHRESH1_STEP1_14 = 0x0E;
        const int RF_AGCTHRESH1_STEP1_15 = 0x0F;
        const int RF_AGCTHRESH1_STEP1_16 = 0x10;  // Default
        const int RF_AGCTHRESH1_STEP1_17 = 0x11;
        const int RF_AGCTHRESH1_STEP1_18 = 0x12;
        const int RF_AGCTHRESH1_STEP1_19 = 0x13;
        const int RF_AGCTHRESH1_STEP1_20 = 0x14;
        const int RF_AGCTHRESH1_STEP1_21 = 0x15;
        const int RF_AGCTHRESH1_STEP1_22 = 0x16;
        const int RF_AGCTHRESH1_STEP1_23 = 0x17;
        const int RF_AGCTHRESH1_STEP1_24 = 0x18;
        const int RF_AGCTHRESH1_STEP1_25 = 0x19;
        const int RF_AGCTHRESH1_STEP1_26 = 0x1A;
        const int RF_AGCTHRESH1_STEP1_27 = 0x1B;
        const int RF_AGCTHRESH1_STEP1_28 = 0x1C;
        const int RF_AGCTHRESH1_STEP1_29 = 0x1D;
        const int RF_AGCTHRESH1_STEP1_30 = 0x1E;
        const int RF_AGCTHRESH1_STEP1_31 = 0x1F;


        // RegAgcThresh2 - not present on RFM69/SX1231
        const int RF_AGCTHRESH2_STEP2_0 = 0x00;
        const int RF_AGCTHRESH2_STEP2_1 = 0x10;
        const int RF_AGCTHRESH2_STEP2_2 = 0x20;
        const int RF_AGCTHRESH2_STEP2_3 = 0x30;  // XXX wrong -- Default
        const int RF_AGCTHRESH2_STEP2_4 = 0x40;
        const int RF_AGCTHRESH2_STEP2_5 = 0x50;
        const int RF_AGCTHRESH2_STEP2_6 = 0x60;
        const int RF_AGCTHRESH2_STEP2_7 = 0x70;  // default
        const int RF_AGCTHRESH2_STEP2_8 = 0x80;
        const int RF_AGCTHRESH2_STEP2_9 = 0x90;
        const int RF_AGCTHRESH2_STEP2_10 = 0xA0;
        const int RF_AGCTHRESH2_STEP2_11 = 0xB0;
        const int RF_AGCTHRESH2_STEP2_12 = 0xC0;
        const int RF_AGCTHRESH2_STEP2_13 = 0xD0;
        const int RF_AGCTHRESH2_STEP2_14 = 0xE0;
        const int RF_AGCTHRESH2_STEP2_15 = 0xF0;

        const int RF_AGCTHRESH2_STEP3_0 = 0x00;
        const int RF_AGCTHRESH2_STEP3_1 = 0x01;
        const int RF_AGCTHRESH2_STEP3_2 = 0x02;
        const int RF_AGCTHRESH2_STEP3_3 = 0x03;
        const int RF_AGCTHRESH2_STEP3_4 = 0x04;
        const int RF_AGCTHRESH2_STEP3_5 = 0x05;
        const int RF_AGCTHRESH2_STEP3_6 = 0x06;
        const int RF_AGCTHRESH2_STEP3_7 = 0x07;
        const int RF_AGCTHRESH2_STEP3_8 = 0x08;
        const int RF_AGCTHRESH2_STEP3_9 = 0x09;
        const int RF_AGCTHRESH2_STEP3_10 = 0x0A;
        const int RF_AGCTHRESH2_STEP3_11 = 0x0B;  // Default
        const int RF_AGCTHRESH2_STEP3_12 = 0x0C;
        const int RF_AGCTHRESH2_STEP3_13 = 0x0D;
        const int RF_AGCTHRESH2_STEP3_14 = 0x0E;
        const int RF_AGCTHRESH2_STEP3_15 = 0x0F;


        // RegAgcThresh3 - not present on RFM69/SX1231
        const int RF_AGCTHRESH3_STEP4_0 = 0x00;
        const int RF_AGCTHRESH3_STEP4_1 = 0x10;
        const int RF_AGCTHRESH3_STEP4_2 = 0x20;
        const int RF_AGCTHRESH3_STEP4_3 = 0x30;
        const int RF_AGCTHRESH3_STEP4_4 = 0x40;
        const int RF_AGCTHRESH3_STEP4_5 = 0x50;
        const int RF_AGCTHRESH3_STEP4_6 = 0x60;
        const int RF_AGCTHRESH3_STEP4_7 = 0x70;
        const int RF_AGCTHRESH3_STEP4_8 = 0x80;
        const int RF_AGCTHRESH3_STEP4_9 = 0x90;  // Default
        const int RF_AGCTHRESH3_STEP4_10 = 0xA0;
        const int RF_AGCTHRESH3_STEP4_11 = 0xB0;
        const int RF_AGCTHRESH3_STEP4_12 = 0xC0;
        const int RF_AGCTHRESH3_STEP4_13 = 0xD0;
        const int RF_AGCTHRESH3_STEP4_14 = 0xE0;
        const int RF_AGCTHRESH3_STEP4_15 = 0xF0;

        const int RF_AGCTHRESH3_STEP5_0 = 0x00;
        const int RF_AGCTHRESH3_STEP5_1 = 0x01;
        const int RF_AGCTHRESH3_STEP5_2 = 0x02;
        const int RF_AGCTHRESH3_STEP5_3 = 0x03;
        const int RF_AGCTHRESH3_STEP5_4 = 0x04;
        const int RF_AGCTHRESH3_STEP5_5 = 0x05;
        const int RF_AGCTHRESH3_STEP5_6 = 0x06;
        const int RF_AGCTHRESH3_STEP5_7 = 0x07;
        const int RF_AGCTHRES33_STEP5_8 = 0x08;
        const int RF_AGCTHRESH3_STEP5_9 = 0x09;
        const int RF_AGCTHRESH3_STEP5_10 = 0x0A;
        const int RF_AGCTHRESH3_STEP5_11 = 0x0B;  // Default
        const int RF_AGCTHRESH3_STEP5_12 = 0x0C;
        const int RF_AGCTHRESH3_STEP5_13 = 0x0D;
        const int RF_AGCTHRESH3_STEP5_14 = 0x0E;
        const int RF_AGCTHRESH3_STEP5_15 = 0x0F;


        // RegLna
        const int RF_LNA_ZIN_50 = 0x00;  // Reset value
        const int RF_LNA_ZIN_200 = 0x80;  // Recommended default

        const int RF_LNA_LOWPOWER_OFF = 0x00;  // Default
        const int RF_LNA_LOWPOWER_ON = 0x40;

        const int RF_LNA_CURRENTGAIN = 0x08;

        const int RF_LNA_GAINSELECT_AUTO = 0x00;  // Default
        const int RF_LNA_GAINSELECT_MAX = 0x01;
        const int RF_LNA_GAINSELECT_MAXMINUS6 = 0x02;
        const int RF_LNA_GAINSELECT_MAXMINUS12 = 0x03;
        const int RF_LNA_GAINSELECT_MAXMINUS24 = 0x04;
        const int RF_LNA_GAINSELECT_MAXMINUS36 = 0x05;
        const int RF_LNA_GAINSELECT_MAXMINUS48 = 0x06;


        // RegRxBw
        const int RF_RXBW_DCCFREQ_000 = 0x00;
        const int RF_RXBW_DCCFREQ_001 = 0x20;
        const int RF_RXBW_DCCFREQ_010 = 0x40;  // Recommended default
        const int RF_RXBW_DCCFREQ_011 = 0x60;
        const int RF_RXBW_DCCFREQ_100 = 0x80;  // Reset value
        const int RF_RXBW_DCCFREQ_101 = 0xA0;
        const int RF_RXBW_DCCFREQ_110 = 0xC0;
        const int RF_RXBW_DCCFREQ_111 = 0xE0;

        const int RF_RXBW_MANT_16 = 0x00;  // Reset value
        const int RF_RXBW_MANT_20 = 0x08;
        const int RF_RXBW_MANT_24 = 0x10;  // Recommended default

        const int RF_RXBW_EXP_0 = 0x00;
        const int RF_RXBW_EXP_1 = 0x01;
        const int RF_RXBW_EXP_2 = 0x02;
        const int RF_RXBW_EXP_3 = 0x03;
        const int RF_RXBW_EXP_4 = 0x04;
        const int RF_RXBW_EXP_5 = 0x05;  // Recommended default
        const int RF_RXBW_EXP_6 = 0x06;  // Reset value
        const int RF_RXBW_EXP_7 = 0x07;


        // RegAfcBw
        const int RF_AFCBW_DCCFREQAFC_000 = 0x00;
        const int RF_AFCBW_DCCFREQAFC_001 = 0x20;
        const int RF_AFCBW_DCCFREQAFC_010 = 0x40;
        const int RF_AFCBW_DCCFREQAFC_011 = 0x60;
        const int RF_AFCBW_DCCFREQAFC_100 = 0x80;  // Default
        const int RF_AFCBW_DCCFREQAFC_101 = 0xA0;
        const int RF_AFCBW_DCCFREQAFC_110 = 0xC0;
        const int RF_AFCBW_DCCFREQAFC_111 = 0xE0;

        const int RF_AFCBW_MANTAFC_16 = 0x00;
        const int RF_AFCBW_MANTAFC_20 = 0x08;  // Default
        const int RF_AFCBW_MANTAFC_24 = 0x10;

        const int RF_AFCBW_EXPAFC_0 = 0x00;
        const int RF_AFCBW_EXPAFC_1 = 0x01;
        const int RF_AFCBW_EXPAFC_2 = 0x02;  // Reset value
        const int RF_AFCBW_EXPAFC_3 = 0x03;  // Recommended default
        const int RF_AFCBW_EXPAFC_4 = 0x04;
        const int RF_AFCBW_EXPAFC_5 = 0x05;
        const int RF_AFCBW_EXPAFC_6 = 0x06;
        const int RF_AFCBW_EXPAFC_7 = 0x07;


        // RegOokPeak
        const int RF_OOKPEAK_THRESHTYPE_FIXED = 0x00;
        const int RF_OOKPEAK_THRESHTYPE_PEAK = 0x40;  // Default
        const int RF_OOKPEAK_THRESHTYPE_AVERAGE = 0x80;

        const int RF_OOKPEAK_PEAKTHRESHSTEP_000 = 0x00;  // Default
        const int RF_OOKPEAK_PEAKTHRESHSTEP_001 = 0x08;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_010 = 0x10;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_011 = 0x18;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_100 = 0x20;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_101 = 0x28;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_110 = 0x30;
        const int RF_OOKPEAK_PEAKTHRESHSTEP_111 = 0x38;

        const int RF_OOKPEAK_PEAKTHRESHDEC_000 = 0x00;  // Default
        const int RF_OOKPEAK_PEAKTHRESHDEC_001 = 0x01;
        const int RF_OOKPEAK_PEAKTHRESHDEC_010 = 0x02;
        const int RF_OOKPEAK_PEAKTHRESHDEC_011 = 0x03;
        const int RF_OOKPEAK_PEAKTHRESHDEC_100 = 0x04;
        const int RF_OOKPEAK_PEAKTHRESHDEC_101 = 0x05;
        const int RF_OOKPEAK_PEAKTHRESHDEC_110 = 0x06;
        const int RF_OOKPEAK_PEAKTHRESHDEC_111 = 0x07;


        // RegOokAvg
        const int RF_OOKAVG_AVERAGETHRESHFILT_00 = 0x00;
        const int RF_OOKAVG_AVERAGETHRESHFILT_01 = 0x40;
        const int RF_OOKAVG_AVERAGETHRESHFILT_10 = 0x80;  // Default
        const int RF_OOKAVG_AVERAGETHRESHFILT_11 = 0xC0;


        // RegOokFix
        const int RF_OOKFIX_FIXEDTHRESH_VALUE = 0x06;  // Default


        // RegAfcFei
        const int RF_AFCFEI_FEI_DONE = 0x40;
        const int RF_AFCFEI_FEI_START = 0x20;
        const int RF_AFCFEI_AFC_DONE = 0x10;
        const int RF_AFCFEI_AFCAUTOCLEAR_ON = 0x08;
        const int RF_AFCFEI_AFCAUTOCLEAR_OFF = 0x00;  // Default

        const int RF_AFCFEI_AFCAUTO_ON = 0x04;
        const int RF_AFCFEI_AFCAUTO_OFF = 0x00;  // Default

        const int RF_AFCFEI_AFC_CLEAR = 0x02;
        const int RF_AFCFEI_AFC_START = 0x01;


        // RegRssiConfig
        const int RF_RSSI_FASTRX_ON = 0x08;  // not present on RFM69/SX1231
        const int RF_RSSI_FASTRX_OFF = 0x00;  // Default

        const int RF_RSSI_DONE = 0x02;
        const int RF_RSSI_START = 0x01;


        // RegDioMapping1
        const int RF_DIOMAPPING1_DIO0_00 = 0x00;  // Default
        const int RF_DIOMAPPING1_DIO0_01 = 0x40;
        const int RF_DIOMAPPING1_DIO0_10 = 0x80;
        const int RF_DIOMAPPING1_DIO0_11 = 0xC0;

        const int RF_DIOMAPPING1_DIO1_00 = 0x00;  // Default
        const int RF_DIOMAPPING1_DIO1_01 = 0x10;
        const int RF_DIOMAPPING1_DIO1_10 = 0x20;
        const int RF_DIOMAPPING1_DIO1_11 = 0x30;

        const int RF_DIOMAPPING1_DIO2_00 = 0x00;  // Default
        const int RF_DIOMAPPING1_DIO2_01 = 0x04;
        const int RF_DIOMAPPING1_DIO2_10 = 0x08;
        const int RF_DIOMAPPING1_DIO2_11 = 0x0C;

        const int RF_DIOMAPPING1_DIO3_00 = 0x00;  // Default
        const int RF_DIOMAPPING1_DIO3_01 = 0x01;
        const int RF_DIOMAPPING1_DIO3_10 = 0x02;
        const int RF_DIOMAPPING1_DIO3_11 = 0x03;


        // RegDioMapping2
        const int RF_DIOMAPPING2_DIO4_00 = 0x00;  // Default
        const int RF_DIOMAPPING2_DIO4_01 = 0x40;
        const int RF_DIOMAPPING2_DIO4_10 = 0x80;
        const int RF_DIOMAPPING2_DIO4_11 = 0xC0;

        const int RF_DIOMAPPING2_DIO5_00 = 0x00;  // Default
        const int RF_DIOMAPPING2_DIO5_01 = 0x10;
        const int RF_DIOMAPPING2_DIO5_10 = 0x20;
        const int RF_DIOMAPPING2_DIO5_11 = 0x30;

        const int RF_DIOMAPPING2_CLKOUT_32MHZ = 0x00;
        const int RF_DIOMAPPING2_CLKOUT_16MHZ = 0x01;
        const int RF_DIOMAPPING2_CLKOUT_8MHZ = 0x02;
        const int RF_DIOMAPPING2_CLKOUT_4MHZ = 0x03;
        const int RF_DIOMAPPING2_CLKOUT_2MHZ = 0x04;
        const int RF_DIOMAPPING2_CLKOUT_1MHZ = 0x05;  // Reset value
        const int RF_DIOMAPPING2_CLKOUT_RC = 0x06;
        const int RF_DIOMAPPING2_CLKOUT_OFF = 0x07;  // Recommended default


        // RegIrqFlags1
        const int RF_IRQFLAGS1_MODEREADY = 0x80;
        const int RF_IRQFLAGS1_RXREADY = 0x40;
        const int RF_IRQFLAGS1_TXREADY = 0x20;
        const int RF_IRQFLAGS1_PLLLOCK = 0x10;
        const int RF_IRQFLAGS1_RSSI = 0x08;
        const int RF_IRQFLAGS1_TIMEOUT = 0x04;
        const int RF_IRQFLAGS1_AUTOMODE = 0x02;
        const int RF_IRQFLAGS1_SYNCADDRESSMATCH = 0x01;


        // RegIrqFlags2
        const int RF_IRQFLAGS2_FIFOFULL = 0x80;
        const int RF_IRQFLAGS2_FIFONOTEMPTY = 0x40;
        const int RF_IRQFLAGS2_FIFOLEVEL = 0x20;
        const int RF_IRQFLAGS2_FIFOOVERRUN = 0x10;
        const int RF_IRQFLAGS2_PACKETSENT = 0x08;
        const int RF_IRQFLAGS2_PAYLOADREADY = 0x04;
        const int RF_IRQFLAGS2_CRCOK = 0x02;
        const int RF_IRQFLAGS2_LOWBAT = 0x01;  // not present on RFM69/SX1231


        // RegRssiThresh
        const int RF_RSSITHRESH_VALUE = 0xE4;  // Default


        // RegRxTimeout1
        const int RF_RXTIMEOUT1_RXSTART_VALUE = 0x00;  // Default


        // RegRxTimeout2
        const int RF_RXTIMEOUT2_RSSITHRESH_VALUE = 0x00;  // Default


        // RegPreamble
        const int RF_PREAMBLESIZE_MSB_VALUE = 0x00;  // Default
        const int RF_PREAMBLESIZE_LSB_VALUE = 0x03;  // Default


        // RegSyncConfig
        const int RF_SYNC_ON = 0x80;  // Default
        const int RF_SYNC_OFF = 0x00;

        const int RF_SYNC_FIFOFILL_AUTO = 0x00;  // Default -- when sync interrupt occurs
        const int RF_SYNC_FIFOFILL_MANUAL = 0x40;

        const int RF_SYNC_SIZE_1 = 0x00;
        const int RF_SYNC_SIZE_2 = 0x08;
        const int RF_SYNC_SIZE_3 = 0x10;
        const int RF_SYNC_SIZE_4 = 0x18;  // Default
        const int RF_SYNC_SIZE_5 = 0x20;
        const int RF_SYNC_SIZE_6 = 0x28;
        const int RF_SYNC_SIZE_7 = 0x30;
        const int RF_SYNC_SIZE_8 = 0x38;

        const int RF_SYNC_TOL_0 = 0x00;  // Default
        const int RF_SYNC_TOL_1 = 0x01;
        const int RF_SYNC_TOL_2 = 0x02;
        const int RF_SYNC_TOL_3 = 0x03;
        const int RF_SYNC_TOL_4 = 0x04;
        const int RF_SYNC_TOL_5 = 0x05;
        const int RF_SYNC_TOL_6 = 0x06;
        const int RF_SYNC_TOL_7 = 0x07;


        // RegSyncValue1-8
        const int RF_SYNC_BYTE1_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE2_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE3_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE4_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE5_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE6_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE7_VALUE = 0x00;  // Default
        const int RF_SYNC_BYTE8_VALUE = 0x00;  // Default


        // RegPacketConfig1
        const int RF_PACKET1_FORMAT_FIXED = 0x00;  // Default
        const int RF_PACKET1_FORMAT_VARIABLE = 0x80;

        const int RF_PACKET1_DCFREE_OFF = 0x00;  // Default
        const int RF_PACKET1_DCFREE_MANCHESTER = 0x20;
        const int RF_PACKET1_DCFREE_WHITENING = 0x40;

        const int RF_PACKET1_CRC_ON = 0x10;  // Default
        const int RF_PACKET1_CRC_OFF = 0x00;

        const int RF_PACKET1_CRCAUTOCLEAR_ON = 0x00;  // Default
        const int RF_PACKET1_CRCAUTOCLEAR_OFF = 0x08;

        const int RF_PACKET1_ADRSFILTERING_OFF = 0x00;  // Default
        const int RF_PACKET1_ADRSFILTERING_NODE = 0x02;
        const int RF_PACKET1_ADRSFILTERING_NODEBROADCAST = 0x04;


        // RegPayloadLength
        const int RF_PAYLOADLENGTH_VALUE = 0x40;  // Default


        // RegBroadcastAdrs
        const int RF_BROADCASTADDRESS_VALUE = 0x00;


        // RegAutoModes
        const int RF_AUTOMODES_ENTER_OFF = 0x00;  // Default
        const int RF_AUTOMODES_ENTER_FIFONOTEMPTY = 0x20;
        const int RF_AUTOMODES_ENTER_FIFOLEVEL = 0x40;
        const int RF_AUTOMODES_ENTER_CRCOK = 0x60;
        const int RF_AUTOMODES_ENTER_PAYLOADREADY = 0x80;
        const int RF_AUTOMODES_ENTER_SYNCADRSMATCH = 0xA0;
        const int RF_AUTOMODES_ENTER_PACKETSENT = 0xC0;
        const int RF_AUTOMODES_ENTER_FIFOEMPTY = 0xE0;

        const int RF_AUTOMODES_EXIT_OFF = 0x00;  // Default
        const int RF_AUTOMODES_EXIT_FIFOEMPTY = 0x04;
        const int RF_AUTOMODES_EXIT_FIFOLEVEL = 0x08;
        const int RF_AUTOMODES_EXIT_CRCOK = 0x0C;
        const int RF_AUTOMODES_EXIT_PAYLOADREADY = 0x10;
        const int RF_AUTOMODES_EXIT_SYNCADRSMATCH = 0x14;
        const int RF_AUTOMODES_EXIT_PACKETSENT = 0x18;
        const int RF_AUTOMODES_EXIT_RXTIMEOUT = 0x1C;

        const int RF_AUTOMODES_INTERMEDIATE_SLEEP = 0x00;  // Default
        const int RF_AUTOMODES_INTERMEDIATE_STANDBY = 0x01;
        const int RF_AUTOMODES_INTERMEDIATE_RECEIVER = 0x02;
        const int RF_AUTOMODES_INTERMEDIATE_TRANSMITTER = 0x03;


        // RegFifoThresh
        const int RF_FIFOTHRESH_TXSTART_FIFOTHRESH = 0x00;  // Reset value
        const int RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY = 0x80;  // Recommended default

        const int RF_FIFOTHRESH_VALUE = 0x0F;  // Default


        // RegPacketConfig2
        const int RF_PACKET2_RXRESTARTDELAY_1BIT = 0x00;  // Default
        const int RF_PACKET2_RXRESTARTDELAY_2BITS = 0x10;
        const int RF_PACKET2_RXRESTARTDELAY_4BITS = 0x20;
        const int RF_PACKET2_RXRESTARTDELAY_8BITS = 0x30;
        const int RF_PACKET2_RXRESTARTDELAY_16BITS = 0x40;
        const int RF_PACKET2_RXRESTARTDELAY_32BITS = 0x50;
        const int RF_PACKET2_RXRESTARTDELAY_64BITS = 0x60;
        const int RF_PACKET2_RXRESTARTDELAY_128BITS = 0x70;
        const int RF_PACKET2_RXRESTARTDELAY_256BITS = 0x80;
        const int RF_PACKET2_RXRESTARTDELAY_512BITS = 0x90;
        const int RF_PACKET2_RXRESTARTDELAY_1024BITS = 0xA0;
        const int RF_PACKET2_RXRESTARTDELAY_2048BITS = 0xB0;
        const int RF_PACKET2_RXRESTARTDELAY_NONE = 0xC0;
        const int RF_PACKET2_RXRESTART = 0x04;

        const int RF_PACKET2_AUTORXRESTART_ON = 0x02;  // Default
        const int RF_PACKET2_AUTORXRESTART_OFF = 0x00;

        const int RF_PACKET2_AES_ON = 0x01;
        const int RF_PACKET2_AES_OFF = 0x00;  // Default


        // RegAesKey1-16
        const int RF_AESKEY1_VALUE = 0x00;  // Default
        const int RF_AESKEY2_VALUE = 0x00;  // Default
        const int RF_AESKEY3_VALUE = 0x00;  // Default
        const int RF_AESKEY4_VALUE = 0x00;  // Default
        const int RF_AESKEY5_VALUE = 0x00;  // Default
        const int RF_AESKEY6_VALUE = 0x00;  // Default
        const int RF_AESKEY7_VALUE = 0x00;  // Default
        const int RF_AESKEY8_VALUE = 0x00;  // Default
        const int RF_AESKEY9_VALUE = 0x00;  // Default
        const int RF_AESKEY10_VALUE = 0x00;  // Default
        const int RF_AESKEY11_VALUE = 0x00;  // Default
        const int RF_AESKEY12_VALUE = 0x00;  // Default
        const int RF_AESKEY13_VALUE = 0x00;  // Default
        const int RF_AESKEY14_VALUE = 0x00;  // Default
        const int RF_AESKEY15_VALUE = 0x00;  // Default
        const int RF_AESKEY16_VALUE = 0x00;  // Default


        // RegTemp1
        const int RF_TEMP1_MEAS_START = 0x08;
        const int RF_TEMP1_MEAS_RUNNING = 0x04;
        // not present on RFM69/SX1231
        const int RF_TEMP1_ADCLOWPOWER_ON = 0x01;  // Default
        const int RF_TEMP1_ADCLOWPOWER_OFF = 0x00;


        // RegTestLna
        const int RF_TESTLNA_NORMAL = 0x1B;
        const int RF_TESTLNA_HIGH_SENSITIVITY = 0x2D;


        // RegTestDagc
        const int RF_DAGC_NORMAL = 0x00;  // Reset value
        const int RF_DAGC_IMPROVED_LOWBETA1 = 0x20;
        const int RF_DAGC_IMPROVED_LOWBETA0 = 0x30;  // Recommended default
        #endregion
        #region RFM69
        // **********************************************************************************
        // Driver definition for HopeRF RFM69W/RFM69HW/RFM69CW/RFM69HCW, Semtech SX1231/1231H
        // **********************************************************************************
        // Copyright Felix Rusu 2016, http://www.LowPowerLab.com/contact
        // **********************************************************************************
        // License
        // **********************************************************************************
        // This program is free software; you can redistribute it 
        // and/or modify it under the terms of the GNU General    
        // Public License as published by the Free Software       
        // Foundation; either version 3 of the License, or        
        // (at your option) any later version.                    
        //                                                        
        // This program is distributed in the hope that it will   
        // be useful, but WITHOUT ANY WARRANTY; without even the  
        // implied warranty of MERCHANTABILITY or FITNESS FOR A   
        // PARTICULAR PURPOSE. See the GNU General Public        
        // License for more details.                              
        //                                                        
        // Licence can be viewed at                               
        // http://www.gnu.org/licenses/gpl-3.0.txt
        //
        // Please maintain this license information along with authorship
        // and copyright notices in any redistribution of this code
        // **********************************************************************************
        const int RF69_MAX_DATA_LEN = 61; // to take advantage of the built in AES/CRC we want to limit the frame size to the internal FIFO size (66 bytes - 3 bytes overhead - 2 bytes crc)
                                          //#define RF69_SPI_CS             SS // SS is the SPI slave select pin, for instance D10 on ATmega328

        // INT0 on AVRs should be connected to RFM69's DIO0 (ex on ATmega328 it's D2, on ATmega644/1284 it's D2)
        /*#if defined(__AVR_ATmega168__) || defined(__AVR_ATmega328P__) || defined(__AVR_ATmega88) || defined(__AVR_ATmega8__) || defined(__AVR_ATmega88__)
        const int RF69_IRQ_PIN = 2;
        const int RF69_IRQ_NUM = 0;
        #elif defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)
        const int RF69_IRQ_PIN = 2;
        const int RF69_IRQ_NUM = 2;
        #elif defined(__AVR_ATmega32U4__)
        const int RF69_IRQ_PIN = 3;
        const int RF69_IRQ_NUM = 0;
        #elif defined(__arm__)//Use pin 10 or any pin you want
        const int RF69_IRQ_PIN = 10;
        const int RF69_IRQ_NUM = 10;
        #else
        const int RF69_IRQ_PIN = 2;
        const int RF69_IRQ_NUM = 0;
        #endif*/


        const int CSMA_LIMIT = -90; // upper RX signal sensitivity threshold in dBm for carrier sense access
        const int RF69_MODE_SLEEP = 0; // XTAL OFF
        const int RF69_MODE_STANDBY = 1; // XTAL ON
        const int RF69_MODE_SYNTH = 2; // PLL ON
        const int RF69_MODE_RX = 3;// RX MODE
        const int RF69_MODE_TX = 4; // TX MODE

        // available frequency bands
        const int RF69_315MHZ = 31; // non trivial values to avoid misconfiguration
        const int RF69_433MHZ = 43;
        const int RF69_868MHZ = 86;
        const int RF69_915MHZ = 91;

        const int COURSE_TEMP_COEF = -90; // puts the temperature reading in the ballpark, user can fine tune the returned value
        const int RF69_BROADCAST_ADDR = 255;
        const int RF69_CSMA_LIMIT_MS = 1000;
        const int RF69_TX_LIMIT_MS = 1000;
        const double RF69_FSTEP = 61.03515625; // == FXOSC / 2^19 = 32MHz / 2^19 (p13 in datasheet)

        // TWS: define CTLbyte bits
        const int RFM69_CTL_SENDACK = 0x80;
        const int RFM69_CTL_REQACK = 0x40;

        public static byte[] DATA;
        public static byte DATALEN;
        public static byte SENDERID;
        public static byte TARGETID; // should match _address
        public static byte PAYLOADLEN;
        public static byte ACK_REQUESTED;
        public static byte ACK_RECEIVED; // should be polled immediately after sending a packet with ACK request
        static volatile short RSSI; // most accurate RSSI during reception (closest to the reception)
        public static byte _mode; // should be protected?

        //RFM69(uint8_t slaveSelectPin = RF69_SPI_CS, uint8_t interruptPin = RF69_IRQ_PIN, bool isRFM69HW = false, uint8_t interruptNum = RF69_IRQ_NUM)
        public RFM69(byte slaveSelectPin = 0, byte interruptPin = 0, bool isRFM69HW = true, byte interruptNum = 0)
        {
            _slaveSelectPin = slaveSelectPin;
            _interruptPin = interruptPin;
            _interruptNum = interruptNum;
            _mode = RF69_MODE_STANDBY;
            _promiscuousMode = false;
            _powerLevel = 31;
            _isRFM69HW = isRFM69HW;
        }

        private long millis()
        {
            return DateTime.Now.Ticks / TimeSpan.TicksPerMillisecond;
        }
        public bool initialize(byte freqBand, byte nodeID, byte networkID = 1)
        {
            byte[][] CONFIG = new byte[][]
	        {
		        /* 0x01 */new byte[] { REG_OPMODE, RF_OPMODE_SEQUENCER_ON | RF_OPMODE_LISTEN_OFF | RF_OPMODE_STANDBY },
		        /* 0x02 */new byte[] { REG_DATAMODUL, RF_DATAMODUL_DATAMODE_PACKET | RF_DATAMODUL_MODULATIONTYPE_FSK | RF_DATAMODUL_MODULATIONSHAPING_00 }, // no shaping
		        /* 0x03 */new byte[] { REG_BITRATEMSB, RF_BITRATEMSB_55555 }, // default: 4.8 KBPS
		        /* 0x04 */new byte[] { REG_BITRATELSB, RF_BITRATELSB_55555 },
		        /* 0x05 */new byte[] { REG_FDEVMSB, RF_FDEVMSB_50000 }, // default: 5KHz, (FDEV + BitRate / 2 <= 500KHz)
		        /* 0x06 */new byte[] { REG_FDEVLSB, RF_FDEVLSB_50000 },

		        /* 0x07 */new byte[] { REG_FRFMSB, (byte)(freqBand == RF69_315MHZ? RF_FRFMSB_315 : (freqBand == RF69_433MHZ? RF_FRFMSB_433 : (freqBand == RF69_868MHZ? RF_FRFMSB_868 : RF_FRFMSB_915))) },
		        /* 0x08 */new byte[] { REG_FRFMID, (byte)(freqBand == RF69_315MHZ? RF_FRFMID_315 : (freqBand == RF69_433MHZ? RF_FRFMID_433 : (freqBand == RF69_868MHZ? RF_FRFMID_868 : RF_FRFMID_915))) },
		        /* 0x09 */new byte[] { REG_FRFLSB, (byte)(freqBand == RF69_315MHZ? RF_FRFLSB_315 : (freqBand == RF69_433MHZ? RF_FRFLSB_433 : (freqBand == RF69_868MHZ? RF_FRFLSB_868 : RF_FRFLSB_915))) },

		        // looks like PA1 and PA2 are not implemented on RFM69W, hence the max output power is 13dBm
		        // +17dBm and +20dBm are possible on RFM69HW
		        // +13dBm formula: Pout = -18 + OutputPower (with PA0 or PA1**)
		        // +17dBm formula: Pout = -14 + OutputPower (with PA1 and PA2)**
		        // +20dBm formula: Pout = -11 + OutputPower (with PA1 and PA2)** and high power PA settings (section 3.3.7 in datasheet)
		        ///* 0x11 */ { REG_PALEVEL, RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | RF_PALEVEL_OUTPUTPOWER_11111},
		        ///* 0x13 */ { REG_OCP, RF_OCP_ON | RF_OCP_TRIM_95 }, // over current protection (default is 95mA)

		        // RXBW defaults are { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_5} (RxBw: 10.4KHz)
		        /* 0x19 */new byte[] { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_16 | RF_RXBW_EXP_2 }, // (BitRate < 2 * RxBw)
																					           //for BR-19200: /* 0x19 */ { REG_RXBW, RF_RXBW_DCCFREQ_010 | RF_RXBW_MANT_24 | RF_RXBW_EXP_3 },
		        /* 0x25 */new byte[] { REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01 }, // DIO0 is the only IRQ we're using
		        /* 0x26 */new byte[] { REG_DIOMAPPING2, RF_DIOMAPPING2_CLKOUT_OFF }, // DIO5 ClkOut disable for power saving
		        /* 0x28 */new byte[] { REG_IRQFLAGS2, RF_IRQFLAGS2_FIFOOVERRUN }, // writing to this bit ensures that the FIFO & status flags are reset
		        /* 0x29 */new byte[] { REG_RSSITHRESH, 220 }, // must be set to dBm = (-Sensitivity / 2), default is 0xE4 = 228 so -114dBm
										           ///* 0x2D */ { REG_PREAMBLELSB, RF_PREAMBLESIZE_LSB_VALUE } // default 3 preamble bytes 0xAAAAAA
		        /* 0x2E */new byte[] { REG_SYNCCONFIG, RF_SYNC_ON | RF_SYNC_FIFOFILL_AUTO | RF_SYNC_SIZE_2 | RF_SYNC_TOL_0 },
		        /* 0x2F */new byte[] { REG_SYNCVALUE1, 0x2D },      // attempt to make this compatible with sync1 byte of RFM12B lib
		        /* 0x30 */new byte[] { REG_SYNCVALUE2, networkID }, // NETWORK ID
		        /* 0x37 */new byte[] { REG_PACKETCONFIG1, RF_PACKET1_FORMAT_VARIABLE | RF_PACKET1_DCFREE_OFF | RF_PACKET1_CRC_ON | RF_PACKET1_CRCAUTOCLEAR_ON | RF_PACKET1_ADRSFILTERING_OFF },
		        /* 0x38 */new byte[] { REG_PAYLOADLENGTH, 66 }, // in variable length mode: the max frame size, not used in TX
											         ///* 0x39 */ { REG_NODEADRS, nodeID }, // turned off because we're not using address filtering
		        /* 0x3C */new byte[] { REG_FIFOTHRESH, RF_FIFOTHRESH_TXSTART_FIFONOTEMPTY | RF_FIFOTHRESH_VALUE }, // TX on FIFO not empty
		        /* 0x3D */new byte[] { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_2BITS | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
																															         //for BR-19200: /* 0x3D */ { REG_PACKETCONFIG2, RF_PACKET2_RXRESTARTDELAY_NONE | RF_PACKET2_AUTORXRESTART_ON | RF_PACKET2_AES_OFF }, // RXRESTARTDELAY must match transmitter PA ramp-down time (bitrate dependent)
		        /* 0x6F */new byte[] { REG_TESTDAGC, RF_DAGC_IMPROVED_LOWBETA0 }, // run DAGC continuously in RX mode for Fading Margin Improvement, recommended default for AfcLowBetaOn=0
		        new byte[] { 255, 0 }
	        };

            //TODO://digitalWrite(_slaveSelectPin, HIGH);
            //TODO://pinMode(_slaveSelectPin, OUTPUT);
            //TODO://SPI.begin();
            long start = millis();
            byte timeout = 50;
            do {
                writeReg(REG_SYNCVALUE1, 0xAA);
            } while (readReg(REG_SYNCVALUE1) != 0xaa && millis() - start < timeout);
	        start = millis();
            do {
                writeReg(REG_SYNCVALUE1, 0x55);
            } while (readReg(REG_SYNCVALUE1) != 0x55 && millis() - start < timeout);

	        for (byte i = 0; CONFIG[i][0] != 255; i++)
            {
                writeReg(CONFIG[i][0], CONFIG[i][1]);
            }

            // Encryption is persistent between resets and can trip you up during debugging.
            // Disable it during initialization so we always start from a known state.
            encrypt(null);

            setHighPower(_isRFM69HW); // called regardless if it's a RFM69W or RFM69HW
            setMode(RF69_MODE_STANDBY);
            start = millis();
            while (((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) && millis() - start < timeout) { } // wait for ModeReady
	        if (millis() - start >= timeout)
		        return false;
	        _inISR = false;
            //TODO://attachInterrupt(_interruptNum, RFM69::isr0, RISING);

            _address = nodeID;
	        return true;
        }
        public void setAddress(byte addr)
        {
            _address = addr;
            writeReg(REG_NODEADRS, _address);
        }
        public void setNetwork(byte networkID)
        {
            writeReg(REG_SYNCVALUE2, networkID);
        }
        public bool canSend()
        {
            if (_mode == RF69_MODE_RX && PAYLOADLEN == 0 && readRSSI() < CSMA_LIMIT) // if signal stronger than -100dBm is detected assume channel activity
            {
                setMode(RF69_MODE_STANDBY);
                return true;
            }
            return false;
        }
        public void send(byte toAddress, byte[] buffer, byte bufferSize, bool requestACK = false)
        {
            writeReg(REG_PACKETCONFIG2, (byte)((readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART)); // avoid RX deadlocks
            long now = millis();
            while (!canSend() && millis() - now < RF69_CSMA_LIMIT_MS) receiveDone();
            sendFrame(toAddress, buffer, bufferSize, requestACK, false);
        }
        public bool sendWithRetry(byte toAddress, byte[] buffer, byte bufferSize, byte retries = 2, byte retryWaitTime = 40) // 40ms roundtrip req for 61byte packets
        {
            long sentTime;
            for (short i = 0; i <= retries; i++)
            {
                send(toAddress, buffer, bufferSize, true);
                sentTime = millis();
                while (millis() - sentTime < retryWaitTime)
                {
                    if (ACKReceived(toAddress))
                    {
                        //Serial.print(" ~ms:"); Serial.print(millis() - sentTime);
                        return true;
                    }
                }
                //Serial.print(" RETRY#"); Serial.println(i + 1);
            }
            return false;
        }
        public bool receiveDone()
        {
            //TODO://noInterrupts(); // re-enabled in unselect() via setMode() or via receiveBegin()
            if (_mode == RF69_MODE_RX && PAYLOADLEN > 0)
            {
                setMode(RF69_MODE_STANDBY); // enables interrupts
                return true;
            }
            else if (_mode == RF69_MODE_RX) // already in RX no payload yet
            {
                //TODO://interrupts(); // explicitly re-enable interrupts
                return false;
            }
            receiveBegin();
            return false;
        }
        public bool ACKReceived(byte fromNodeID)
        {
            if (receiveDone())
                return (SENDERID == fromNodeID || fromNodeID == RF69_BROADCAST_ADDR) && ACK_RECEIVED>0;
            return false;
        }
        public bool ACKRequested()
        {
            return ACK_REQUESTED>0 && (TARGETID != RF69_BROADCAST_ADDR);
        }
        public void sendACK(byte[] buffer = null, byte bufferSize = 0)
        {
            ACK_REQUESTED = 0;   // TWS added to make sure we don't end up in a timing race and infinite loop sending Acks
            byte sender = SENDERID;
            short _RSSI = RSSI; // save payload received RSSI value
            writeReg(REG_PACKETCONFIG2, (byte)((readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART)); // avoid RX deadlocks
            long now = millis();
            while (!canSend() && millis() - now < RF69_CSMA_LIMIT_MS) receiveDone();
            SENDERID = sender;    // TWS: Restore SenderID after it gets wiped out by receiveDone()
            sendFrame(sender, buffer, bufferSize, false, true);
            RSSI = _RSSI; // restore payload RSSI
        }
        public int getFrequency()
        {
            return (int)(RF69_FSTEP * (((int)readReg(REG_FRFMSB) << 16) + ((short)readReg(REG_FRFMID) << 8) + readReg(REG_FRFLSB)));
        }
        public void setFrequency(int freqHz)
        {
            byte oldMode = _mode;
            if (oldMode == RF69_MODE_TX)
            {
                setMode(RF69_MODE_RX);
            }
            freqHz = (int)(freqHz/RF69_FSTEP); // divide down by FSTEP to get FRF
            writeReg(REG_FRFMSB, (byte)(freqHz >> 16));
            writeReg(REG_FRFMID, (byte)(freqHz >> 8));
            writeReg(REG_FRFLSB, (byte)freqHz);
            if (oldMode == RF69_MODE_RX)
            {
                setMode(RF69_MODE_SYNTH);
            }
            setMode(oldMode);
        }
        public void encrypt(string key)
        {
            setMode(RF69_MODE_STANDBY);
            if (key != null)
            {
                select();
                //TODO://SPI.transfer(REG_AESKEY1 | 0x80);
                for (byte i = 0; i < 16; i++)
                {
                    //TODO://SPI.transfer(key[i]);
                }
                unselect();
            }
            writeReg(REG_PACKETCONFIG2, (byte)((readReg(REG_PACKETCONFIG2) & 0xFE) | (key != null ? 1 : 0)));
        }
        public void setCS(byte newSPISlaveSelect)
        {
            _slaveSelectPin = newSPISlaveSelect;
            //TODO://digitalWrite(_slaveSelectPin, HIGH);
            //TODO://pinMode(_slaveSelectPin, OUTPUT);
        }
        public short readRSSI(bool forceTrigger = false)
        {
            short rssi = 0;
            if (forceTrigger)
            {
                // RSSI trigger not needed if DAGC is in continuous mode
                writeReg(REG_RSSICONFIG, RF_RSSI_START);
                while ((readReg(REG_RSSICONFIG) & RF_RSSI_DONE) == 0x00) ; // wait for RSSI_Ready
            }
            //TODO://rssi = -readReg(REG_RSSIVALUE);
            rssi >>= 1;
            return rssi;
        }
        public void promiscuous(bool onOff = true)
        {
            _promiscuousMode = onOff;
        }
        public virtual void setHighPower(bool onOff = true)
        // has to be called after initialize() for RFM69HW
        {
            _isRFM69HW = onOff;
            writeReg(REG_OCP, (byte)(_isRFM69HW ? RF_OCP_OFF : RF_OCP_ON));
            if (_isRFM69HW) // turning ON
                writeReg(REG_PALEVEL, (byte)((readReg(REG_PALEVEL) & 0x1F) | RF_PALEVEL_PA1_ON | RF_PALEVEL_PA2_ON)); // enable P1 & P2 amplifier stages
            else
                writeReg(REG_PALEVEL, (byte)(RF_PALEVEL_PA0_ON | RF_PALEVEL_PA1_OFF | RF_PALEVEL_PA2_OFF | _powerLevel)); // enable P0 only
        }
        public void setPowerLevel(byte powerLevel) // reduce/increase transmit power level
        {
            _powerLevel = (byte)(powerLevel > 31 ? 31 : powerLevel);
            if (_isRFM69HW) _powerLevel /= 2;
            writeReg(REG_PALEVEL, (byte)((readReg(REG_PALEVEL) & 0xE0) | _powerLevel));
        }
        public void sleep()
        {
            setMode(RF69_MODE_SLEEP);
        }
        public byte readTemperature(byte calFactor = 0)
        // get CMOS temperature (8bit) in celcius
        {
            setMode(RF69_MODE_STANDBY);
            writeReg(REG_TEMP1, RF_TEMP1_MEAS_START);
            while ((readReg(REG_TEMP1) & RF_TEMP1_MEAS_RUNNING)>0) ;
            return (byte)(~readReg(REG_TEMP2) + COURSE_TEMP_COEF + calFactor); // 'complement' corrects the slope, rising temp = rising val
                                                                       // COURSE_TEMP_COEF puts reading in the ballpark, user can add additional correction
        }
        public void rcCalibration()
        // calibrate the internal RC oscillator for use in wide temperature variations - see datasheet section [4.3.5. RC Timer Accuracy]
        {
            writeReg(REG_OSC1, RF_OSC1_RCCAL_START);
            while ((readReg(REG_OSC1) & RF_OSC1_RCCAL_DONE) == 0x00) { }
        }

        // allow hacking registers by making these public
        public byte readReg(byte addr)
        {
            select();
            //TODO://SPI.transfer(addr & 0x7F);
            byte regval = 0;//TODO://SPI.transfer(0);
            unselect();
            return regval;
        }
        public void writeReg(byte addr, byte val)
        {
            select();
            //TODO://SPI.transfer(addr | 0x80);
            //TODO://SPI.transfer(value);
            unselect();
        }

	    protected /*TODO:static*/ void isr0()
        {
            _inISR = true; interruptHandler(); _inISR = false;
        }
        protected void interruptHandler()
        {
            if (_mode == RF69_MODE_RX && (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY)>0)
            {
                //RSSI = readRSSI();
                setMode(RF69_MODE_STANDBY);
                select();
                //TODO://SPI.transfer(REG_FIFO & 0x7F);
                //TODO://PAYLOADLEN = SPI.transfer(0);
                PAYLOADLEN = PAYLOADLEN > 66 ? (byte)66 : PAYLOADLEN; // precaution
                //TODO://TARGETID = SPI.transfer(0);
                if (!(_promiscuousMode || TARGETID == _address || TARGETID == RF69_BROADCAST_ADDR) // match this node's address, or broadcast address or anything in promiscuous mode
                    || PAYLOADLEN < 3) // address situation could receive packets that are malformed and don't fit this libraries extra fields
                {
                    PAYLOADLEN = 0;
                    unselect();
                    receiveBegin();
                    //digitalWrite(4, 0);
                    return;
                }

                DATALEN = (byte)(PAYLOADLEN - 3);
                //TODO://SENDERID = SPI.transfer(0);
                byte CTLbyte = 0;//TODO://SPI.transfer(0);

                ACK_RECEIVED = (byte)(CTLbyte & RFM69_CTL_SENDACK); // extract ACK-received flag
                ACK_REQUESTED = (byte)(CTLbyte & RFM69_CTL_REQACK); // extract ACK-requested flag

                interruptHook(CTLbyte);     // TWS: hook to derived class interrupt function

                for (byte i = 0; i < DATALEN; i++)
                {
                    //TODO://DATA[i] = SPI.transfer(0);
                }
                if (DATALEN < RF69_MAX_DATA_LEN) DATA[DATALEN] = 0; // add null at end of string
                unselect();
                setMode(RF69_MODE_RX);
            }
            RSSI = readRSSI();
        }
        protected void interruptHook(byte CTLbyte)
        {

        }
        protected static bool _inISR;
        protected void sendFrame(byte toAddress, byte[] buffer, byte bufferSize, bool requestACK = false, bool sendACK = false)
        {
            setMode(RF69_MODE_STANDBY); // turn off receiver to prevent reception while filling fifo
            while ((readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) ; // wait for ModeReady
            writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_00); // DIO0 is "Packet Sent"
            if (bufferSize > RF69_MAX_DATA_LEN) bufferSize = RF69_MAX_DATA_LEN;

            // control byte
            byte CTLbyte = 0x00;
            if (sendACK)
                CTLbyte = RFM69_CTL_SENDACK;
            else if (requestACK)
                CTLbyte = RFM69_CTL_REQACK;

            // write to FIFO
            select();
            //TODO://SPI.transfer(REG_FIFO | 0x80);
            //TODO://SPI.transfer(bufferSize + 3);
            //TODO://SPI.transfer(toAddress);
            //TODO://SPI.transfer(_address);
            //TODO://SPI.transfer(CTLbyte);

            for (byte i = 0; i < bufferSize; i++)
            {
                //TODO://SPI.transfer(((uint8_t*)buffer)[i]);
            }

                unselect();

            // no need to wait for transmit mode to be ready since its handled by the radio
            setMode(RF69_MODE_TX);
            long txStart = millis();
            //TODO://while (digitalRead(_interruptPin) == 0 && millis() - txStart < RF69_TX_LIMIT_MS) ; // wait for DIO0 to turn HIGH signalling transmission finish
                                                                                               //while (readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PACKETSENT == 0x00); // wait for ModeReady
            setMode(RF69_MODE_STANDBY);
        }

        protected byte _slaveSelectPin;
        protected byte _interruptPin;
        protected byte _interruptNum;
        protected byte _address;
        protected bool _promiscuousMode;
        protected byte _powerLevel;
        protected bool _isRFM69HW;
/*#if defined (SPCR) && defined (SPSR)
	byte _SPCR;
	byte _SPSR;
#endif*/

        protected void receiveBegin()
        {
            DATALEN = 0;
            SENDERID = 0;
            TARGETID = 0;
            PAYLOADLEN = 0;
            ACK_REQUESTED = 0;
            ACK_RECEIVED = 0;
            RSSI = 0;
            if ((readReg(REG_IRQFLAGS2) & RF_IRQFLAGS2_PAYLOADREADY)>0)
                writeReg(REG_PACKETCONFIG2, (byte)((readReg(REG_PACKETCONFIG2) & 0xFB) | RF_PACKET2_RXRESTART)); // avoid RX deadlocks
            writeReg(REG_DIOMAPPING1, RF_DIOMAPPING1_DIO0_01); // set DIO0 to "PAYLOADREADY" in receive mode
            setMode(RF69_MODE_RX);
        }
        protected void setMode(byte newMode)
        {
            if (newMode == _mode)
                return;

            switch (newMode)
            {
                case RF69_MODE_TX:
                    writeReg(REG_OPMODE, (byte)((readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_TRANSMITTER));
                    if (_isRFM69HW) setHighPowerRegs(true);
                    break;
                case RF69_MODE_RX:
                    writeReg(REG_OPMODE, (byte)((readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_RECEIVER));
                    if (_isRFM69HW) setHighPowerRegs(false);
                    break;
                case RF69_MODE_SYNTH:
                    writeReg(REG_OPMODE, (byte)((readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SYNTHESIZER));
                    break;
                case RF69_MODE_STANDBY:
                    writeReg(REG_OPMODE, (byte)((readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_STANDBY));
                    break;
                case RF69_MODE_SLEEP:
                    writeReg(REG_OPMODE, (byte)((readReg(REG_OPMODE) & 0xE3) | RF_OPMODE_SLEEP));
                    break;
                default:
                    return;
            }

            // we are using packet mode, so this check is not really needed
            // but waiting for mode ready is necessary when going from sleep because the FIFO may not be immediately available from previous mode
            while (_mode == RF69_MODE_SLEEP && (readReg(REG_IRQFLAGS1) & RF_IRQFLAGS1_MODEREADY) == 0x00) { } // wait for ModeReady

            _mode = newMode;
        }
        protected void setHighPowerRegs(bool onOff)
        {
            writeReg(REG_TESTPA1, (byte)(onOff ? 0x5D : 0x55));
            writeReg(REG_TESTPA2, (byte)(onOff ? 0x7C : 0x70));
        }
        protected void select()
        {
            //TODO://noInterrupts();
            /*#if defined (SPCR) && defined (SPSR)
                // save current SPI settings
                _SPCR = SPCR;
                _SPSR = SPSR;
            #endif*/
            // set RFM69 SPI settings
            //TODO://SPI.setDataMode(SPI_MODE0);
            //TODO://SPI.setBitOrder(MSBFIRST);
            //TODO://SPI.setClockDivider(SPI_CLOCK_DIV4); // decided to slow down from DIV2 after SPI stalling in some instances, especially visible on mega1284p when RFM69 and FLASH chip both present
            //TODO://digitalWrite(_slaveSelectPin, LOW);
        }
        protected void unselect()
        {
            //TODO://digitalWrite(_slaveSelectPin, HIGH);
            // restore SPI settings to what they were before talking to RFM69
            /*#if defined (SPCR) && defined (SPSR)
                SPCR = _SPCR;
                SPSR = _SPSR;
            #endif*/
            //TODO://maybeInterrupts();
        }
        protected void maybeInterrupts()
        {
            // Only reenable interrupts if we're not being called from the ISR
            if (!_inISR)
            {
                //TODO://interrupts();
            }
        }
        #endregion
    }
}
