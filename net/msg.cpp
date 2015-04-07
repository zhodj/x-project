
/* node_name -- logical name for a process in the network, e.g node1*/
struct node_name {
    __u8 type;
    ___le32 number;
}__attribute__ ((packed));

/* node_addr -- network address */
struct node_addr {
    __le32 type;
    __le32 id;                  /* pid */
    struct sockaddr_storage in_addr;
}__attribute__ ((packed));

struct node_inst {
    struct node_name name;
    struct node_addr addr;
}__attribute__ ((packed));


#define ENTITY_TYPE_NODE 0x01
#define ENTITY_TYPE_CNTL 0x02

#define ENTITY_TYPE_ANY 0xFF

struct msg_header {
    __le64 seq;
    __le16 priority;
    __le16 version;

    __le32 front_len; /*bytes in main payload*/
    __le32 data_len;  /*bytes of data paylod*/
    __le32 data_off;  /*  */

    __le16 version;
    __le32 crc;
}__attribute__ ((packed));

struct msg_connect
{
    __le32 node_no;
    __le32 seq_no;
    __le32 authritory;
}
