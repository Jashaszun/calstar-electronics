using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using mcp2210_dll_m;

namespace RFM69HCW_Control
{
    static class SPI
    {
        private const ushort DEFAULT_VID = 0x4d8;
        private const ushort DEFAULT_PID = 0xde;

        private static IntPtr handle;
        
        public static int getNumDevices()
        {
            return MCP2210.M_Mcp2210_GetConnectedDevCount(DEFAULT_VID, DEFAULT_PID);
        }

        public static bool connect(uint index)
        {
            StringBuilder path = new StringBuilder();
            int result;

            handle = MCP2210.M_Mcp2210_OpenByIndex(DEFAULT_VID, DEFAULT_PID, index, path);
            result = MCP2210.M_Mcp2210_GetLastError();

            return result == MCP2210.M_E_SUCCESS;
        }
    }
}
