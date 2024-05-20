from trex_stl_lib.api import *

# IMIX profile - involves 19200 streams of UDP packets
# cell = 0-64
# ue = 0-15
# bytes = 84:15% 256:10% 1280:75%

class STLImix(object):

    def __init__ (self):
        # default IP range
        self.ip_range = {'src': {'start': "16.0.0.1", 'end': "16.0.0.254"},
                         'dst': {'start': "48.0.0.1", 'end': "48.0.0.254"}}

    def create_stream (self, pps, vm ):
        size = 1500  #payload size in byte
        # create GTP-U header
        byte0 = 0x30
        byte1 = 0xff
        byte2, byte3 = divmod(size,256)
        byte4 = 0x05  #dir:ul(0), dstport-of-UE-server:1, drb-id:5,    == 0001 0101

        # create Stream List
        streamlist = []

        for cell_id in range(0, 9):
            for ue_id in range(1, 401):
                byte5 = (ue_id>>8) & 0xff
                byte6 = ue_id & 0xff
                byte7 = cell_id
                gtpu_header=struct.pack('8B',byte0,byte1,byte2,byte3,byte4,byte5,byte6,byte7)
                base_pkt = Ether()/IP()/UDP(dport = 2152, sport = 2152)/gtpu_header
                pad = max(0, size) * 'x'
                pkt = STLPktBuilder(pkt = base_pkt/pad,
                                    vm = vm)
                isg = 0.0
                #for case:cell0 has different TPut with other cells
                if cell_id < 6:
                    pps0=pps
                else:
                    pps0=pps<<1
                streamlist.append(STLStream(isg = isg,
                                            packet = pkt,
                                            mode = STLTXCont(pps = pps0)))
                                            #flow_stats = STLFlowLatencyStats(pg_id = cell_id+ue_id)))
        return streamlist

    def get_streams (self, direction = 0, **kwargs):
            if direction == 0:
                src = self.ip_range['src']
                dst = self.ip_range['dst']
            else:
                src = self.ip_range['dst']
                dst = self.ip_range['src']

            # construct the base packet for the profile
            vm = STLVM()

            # define two vars (src and dst)
            vm.var(name="src",min_value=src['start'],max_value=src['end'],size=4,op="inc")
            vm.var(name="dst",min_value=dst['start'],max_value=dst['end'],size=4,op="inc")

            # write them
            vm.write(fv_name="src",pkt_offset= "IP.src")
            vm.write(fv_name="dst",pkt_offset= "IP.dst")

            # fix checksum
            vm.fix_chksum()

            # create imix streams
            return self.create_stream(50, vm)

# dynamic load - used for trex console or simulator
def register():
    return STLImix()



