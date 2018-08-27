using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;

namespace RadioSerial
{
    public static class StaticUtils
    {
        public static void tryInvoke(this Control c, Action a)
        {
            if (c.InvokeRequired)
                c.Invoke(a);
            else a();
        }
        public static void tryAction(this Control c, Action a)
        {
            try { a(); }
            catch (Exception) { }
        }
    }
}
