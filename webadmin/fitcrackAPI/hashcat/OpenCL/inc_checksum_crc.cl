/**
 * Author......: See docs/credits.txt
 * License.....: MIT
 */

#include "inc_vendor.h"
#include "inc_types.h"
#include "inc_platform.h"
#include "inc_common.h"
#include "inc_checksum_crc.h"

CONSTANT_VK u32a icrc32tab[256] =
{
  0x00000000, 0xdb710641, 0x6d930ac3, 0xb6e20c82,
  0xdb261586, 0x005713c7, 0xb6b51f45, 0x6dc41904,
  0x6d3d2d4d, 0xb64c2b0c, 0x00ae278e, 0xdbdf21cf,
  0xb61b38cb, 0x6d6a3e8a, 0xdb883208, 0x00f93449,
  0xda7a5a9a, 0x010b5cdb, 0xb7e95059, 0x6c985618,
  0x015c4f1c, 0xda2d495d, 0x6ccf45df, 0xb7be439e,
  0xb74777d7, 0x6c367196, 0xdad47d14, 0x01a57b55,
  0x6c616251, 0xb7106410, 0x01f26892, 0xda836ed3,
  0x6f85b375, 0xb4f4b534, 0x0216b9b6, 0xd967bff7,
  0xb4a3a6f3, 0x6fd2a0b2, 0xd930ac30, 0x0241aa71,
  0x02b89e38, 0xd9c99879, 0x6f2b94fb, 0xb45a92ba,
  0xd99e8bbe, 0x02ef8dff, 0xb40d817d, 0x6f7c873c,
  0xb5ffe9ef, 0x6e8eefae, 0xd86ce32c, 0x031de56d,
  0x6ed9fc69, 0xb5a8fa28, 0x034af6aa, 0xd83bf0eb,
  0xd8c2c4a2, 0x03b3c2e3, 0xb551ce61, 0x6e20c820,
  0x03e4d124, 0xd895d765, 0x6e77dbe7, 0xb506dda6,
  0xdf0b66ea, 0x047a60ab, 0xb2986c29, 0x69e96a68,
  0x042d736c, 0xdf5c752d, 0x69be79af, 0xb2cf7fee,
  0xb2364ba7, 0x69474de6, 0xdfa54164, 0x04d44725,
  0x69105e21, 0xb2615860, 0x048354e2, 0xdff252a3,
  0x05713c70, 0xde003a31, 0x68e236b3, 0xb39330f2,
  0xde5729f6, 0x05262fb7, 0xb3c42335, 0x68b52574,
  0x684c113d, 0xb33d177c, 0x05df1bfe, 0xdeae1dbf,
  0xb36a04bb, 0x681b02fa, 0xdef90e78, 0x05880839,
  0xb08ed59f, 0x6bffd3de, 0xdd1ddf5c, 0x066cd91d,
  0x6ba8c019, 0xb0d9c658, 0x063bcada, 0xdd4acc9b,
  0xddb3f8d2, 0x06c2fe93, 0xb020f211, 0x6b51f450,
  0x0695ed54, 0xdde4eb15, 0x6b06e797, 0xb077e1d6,
  0x6af48f05, 0xb1858944, 0x076785c6, 0xdc168387,
  0xb1d29a83, 0x6aa39cc2, 0xdc419040, 0x07309601,
  0x07c9a248, 0xdcb8a409, 0x6a5aa88b, 0xb12baeca,
  0xdcefb7ce, 0x079eb18f, 0xb17cbd0d, 0x6a0dbb4c,
  0x6567cb95, 0xbe16cdd4, 0x08f4c156, 0xd385c717,
  0xbe41de13, 0x6530d852, 0xd3d2d4d0, 0x08a3d291,
  0x085ae6d8, 0xd32be099, 0x65c9ec1b, 0xbeb8ea5a,
  0xd37cf35e, 0x080df51f, 0xbeeff99d, 0x659effdc,
  0xbf1d910f, 0x646c974e, 0xd28e9bcc, 0x09ff9d8d,
  0x643b8489, 0xbf4a82c8, 0x09a88e4a, 0xd2d9880b,
  0xd220bc42, 0x0951ba03, 0xbfb3b681, 0x64c2b0c0,
  0x0906a9c4, 0xd277af85, 0x6495a307, 0xbfe4a546,
  0x0ae278e0, 0xd1937ea1, 0x67717223, 0xbc007462,
  0xd1c46d66, 0x0ab56b27, 0xbc5767a5, 0x672661e4,
  0x67df55ad, 0xbcae53ec, 0x0a4c5f6e, 0xd13d592f,
  0xbcf9402b, 0x6788466a, 0xd16a4ae8, 0x0a1b4ca9,
  0xd098227a, 0x0be9243b, 0xbd0b28b9, 0x667a2ef8,
  0x0bbe37fc, 0xd0cf31bd, 0x662d3d3f, 0xbd5c3b7e,
  0xbda50f37, 0x66d40976, 0xd03605f4, 0x0b4703b5,
  0x66831ab1, 0xbdf21cf0, 0x0b101072, 0xd0611633,
  0xba6cad7f, 0x611dab3e, 0xd7ffa7bc, 0x0c8ea1fd,
  0x614ab8f9, 0xba3bbeb8, 0x0cd9b23a, 0xd7a8b47b,
  0xd7518032, 0x0c208673, 0xbac28af1, 0x61b38cb0,
  0x0c7795b4, 0xd70693f5, 0x61e49f77, 0xba959936,
  0x6016f7e5, 0xbb67f1a4, 0x0d85fd26, 0xd6f4fb67,
  0xbb30e263, 0x6041e422, 0xd6a3e8a0, 0x0dd2eee1,
  0x0d2bdaa8, 0xd65adce9, 0x60b8d06b, 0xbbc9d62a,
  0xd60dcf2e, 0x0d7cc96f, 0xbb9ec5ed, 0x60efc3ac,
  0xd5e91e0a, 0x0e98184b, 0xb87a14c9, 0x630b1288,
  0x0ecf0b8c, 0xd5be0dcd, 0x635c014f, 0xb82d070e,
  0xb8d43347, 0x63a53506, 0xd5473984, 0x0e363fc5,
  0x63f226c1, 0xb8832080, 0x0e612c02, 0xd5102a43,
  0x0f934490, 0xd4e242d1, 0x62004e53, 0xb9714812,
  0xd4b55116, 0x0fc45757, 0xb9265bd5, 0x62575d94,
  0x62ae69dd, 0xb9df6f9c, 0x0f3d631e, 0xd44c655f,
  0xb9887c5b, 0x62f97a1a, 0xd41b7698, 0x0f6a70d9,
};

CONSTANT_VK u32a crc32tab[256] =
{
  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba,
  0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988,
  0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de,
  0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec,
  0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172,
  0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940,
  0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116,
  0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924,
  0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a,
  0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818,
  0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e,
  0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c,
  0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2,
  0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0,
  0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086,
  0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4,
  0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a,
  0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8,
  0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe,
  0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc,
  0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252,
  0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60,
  0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236,
  0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04,
  0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a,
  0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38,
  0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e,
  0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c,
  0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2,
  0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0,
  0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6,
  0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94,
  0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
};

CONSTANT_VK u32a crc32ctab[0x100] =
{
  0x00000000, 0xf26b8303, 0xe13b70f7, 0x1350f3f4,
  0xc79a971f, 0x35f1141c, 0x26a1e7e8, 0xd4ca64eb,
  0x8ad958cf, 0x78b2dbcc, 0x6be22838, 0x9989ab3b,
  0x4d43cfd0, 0xbf284cd3, 0xac78bf27, 0x5e133c24,
  0x105ec76f, 0xe235446c, 0xf165b798, 0x030e349b,
  0xd7c45070, 0x25afd373, 0x36ff2087, 0xc494a384,
  0x9a879fa0, 0x68ec1ca3, 0x7bbcef57, 0x89d76c54,
  0x5d1d08bf, 0xaf768bbc, 0xbc267848, 0x4e4dfb4b,
  0x20bd8ede, 0xd2d60ddd, 0xc186fe29, 0x33ed7d2a,
  0xe72719c1, 0x154c9ac2, 0x061c6936, 0xf477ea35,
  0xaa64d611, 0x580f5512, 0x4b5fa6e6, 0xb93425e5,
  0x6dfe410e, 0x9f95c20d, 0x8cc531f9, 0x7eaeb2fa,
  0x30e349b1, 0xc288cab2, 0xd1d83946, 0x23b3ba45,
  0xf779deae, 0x05125dad, 0x1642ae59, 0xe4292d5a,
  0xba3a117e, 0x4851927d, 0x5b016189, 0xa96ae28a,
  0x7da08661, 0x8fcb0562, 0x9c9bf696, 0x6ef07595,
  0x417b1dbc, 0xb3109ebf, 0xa0406d4b, 0x522bee48,
  0x86e18aa3, 0x748a09a0, 0x67dafa54, 0x95b17957,
  0xcba24573, 0x39c9c670, 0x2a993584, 0xd8f2b687,
  0x0c38d26c, 0xfe53516f, 0xed03a29b, 0x1f682198,
  0x5125dad3, 0xa34e59d0, 0xb01eaa24, 0x42752927,
  0x96bf4dcc, 0x64d4cecf, 0x77843d3b, 0x85efbe38,
  0xdbfc821c, 0x2997011f, 0x3ac7f2eb, 0xc8ac71e8,
  0x1c661503, 0xee0d9600, 0xfd5d65f4, 0x0f36e6f7,
  0x61c69362, 0x93ad1061, 0x80fde395, 0x72966096,
  0xa65c047d, 0x5437877e, 0x4767748a, 0xb50cf789,
  0xeb1fcbad, 0x197448ae, 0x0a24bb5a, 0xf84f3859,
  0x2c855cb2, 0xdeeedfb1, 0xcdbe2c45, 0x3fd5af46,
  0x7198540d, 0x83f3d70e, 0x90a324fa, 0x62c8a7f9,
  0xb602c312, 0x44694011, 0x5739b3e5, 0xa55230e6,
  0xfb410cc2, 0x092a8fc1, 0x1a7a7c35, 0xe811ff36,
  0x3cdb9bdd, 0xceb018de, 0xdde0eb2a, 0x2f8b6829,
  0x82f63b78, 0x709db87b, 0x63cd4b8f, 0x91a6c88c,
  0x456cac67, 0xb7072f64, 0xa457dc90, 0x563c5f93,
  0x082f63b7, 0xfa44e0b4, 0xe9141340, 0x1b7f9043,
  0xcfb5f4a8, 0x3dde77ab, 0x2e8e845f, 0xdce5075c,
  0x92a8fc17, 0x60c37f14, 0x73938ce0, 0x81f80fe3,
  0x55326b08, 0xa759e80b, 0xb4091bff, 0x466298fc,
  0x1871a4d8, 0xea1a27db, 0xf94ad42f, 0x0b21572c,
  0xdfeb33c7, 0x2d80b0c4, 0x3ed04330, 0xccbbc033,
  0xa24bb5a6, 0x502036a5, 0x4370c551, 0xb11b4652,
  0x65d122b9, 0x97baa1ba, 0x84ea524e, 0x7681d14d,
  0x2892ed69, 0xdaf96e6a, 0xc9a99d9e, 0x3bc21e9d,
  0xef087a76, 0x1d63f975, 0x0e330a81, 0xfc588982,
  0xb21572c9, 0x407ef1ca, 0x532e023e, 0xa145813d,
  0x758fe5d6, 0x87e466d5, 0x94b49521, 0x66df1622,
  0x38cc2a06, 0xcaa7a905, 0xd9f75af1, 0x2b9cd9f2,
  0xff56bd19, 0x0d3d3e1a, 0x1e6dcdee, 0xec064eed,
  0xc38d26c4, 0x31e6a5c7, 0x22b65633, 0xd0ddd530,
  0x0417b1db, 0xf67c32d8, 0xe52cc12c, 0x1747422f,
  0x49547e0b, 0xbb3ffd08, 0xa86f0efc, 0x5a048dff,
  0x8ecee914, 0x7ca56a17, 0x6ff599e3, 0x9d9e1ae0,
  0xd3d3e1ab, 0x21b862a8, 0x32e8915c, 0xc083125f,
  0x144976b4, 0xe622f5b7, 0xf5720643, 0x07198540,
  0x590ab964, 0xab613a67, 0xb831c993, 0x4a5a4a90,
  0x9e902e7b, 0x6cfbad78, 0x7fab5e8c, 0x8dc0dd8f,
  0xe330a81a, 0x115b2b19, 0x020bd8ed, 0xf0605bee,
  0x24aa3f05, 0xd6c1bc06, 0xc5914ff2, 0x37faccf1,
  0x69e9f0d5, 0x9b8273d6, 0x88d28022, 0x7ab90321,
  0xae7367ca, 0x5c18e4c9, 0x4f48173d, 0xbd23943e,
  0xf36e6f75, 0x0105ec76, 0x12551f82, 0xe03e9c81,
  0x34f4f86a, 0xc69f7b69, 0xd5cf889d, 0x27a40b9e,
  0x79b737ba, 0x8bdcb4b9, 0x988c474d, 0x6ae7c44e,
  0xbe2da0a5, 0x4c4623a6, 0x5f16d052, 0xad7d5351
};

CONSTANT_VK u64a crc64jonestab[256] =
{
  0x0000000000000000, 0x7ad870c830358979,
  0xf5b0e190606b12f2, 0x8f689158505e9b8b,
  0xc038e5739841b68f, 0xbae095bba8743ff6,
  0x358804e3f82aa47d, 0x4f50742bc81f2d04,
  0xab28ecb46814fe75, 0xd1f09c7c5821770c,
  0x5e980d24087fec87, 0x24407dec384a65fe,
  0x6b1009c7f05548fa, 0x11c8790fc060c183,
  0x9ea0e857903e5a08, 0xe478989fa00bd371,
  0x7d08ff3b88be6f81, 0x07d08ff3b88be6f8,
  0x88b81eabe8d57d73, 0xf2606e63d8e0f40a,
  0xbd301a4810ffd90e, 0xc7e86a8020ca5077,
  0x4880fbd87094cbfc, 0x32588b1040a14285,
  0xd620138fe0aa91f4, 0xacf86347d09f188d,
  0x2390f21f80c18306, 0x594882d7b0f40a7f,
  0x1618f6fc78eb277b, 0x6cc0863448deae02,
  0xe3a8176c18803589, 0x997067a428b5bcf0,
  0xfa11fe77117cdf02, 0x80c98ebf2149567b,
  0x0fa11fe77117cdf0, 0x75796f2f41224489,
  0x3a291b04893d698d, 0x40f16bccb908e0f4,
  0xcf99fa94e9567b7f, 0xb5418a5cd963f206,
  0x513912c379682177, 0x2be1620b495da80e,
  0xa489f35319033385, 0xde51839b2936bafc,
  0x9101f7b0e12997f8, 0xebd98778d11c1e81,
  0x64b116208142850a, 0x1e6966e8b1770c73,
  0x8719014c99c2b083, 0xfdc17184a9f739fa,
  0x72a9e0dcf9a9a271, 0x08719014c99c2b08,
  0x4721e43f0183060c, 0x3df994f731b68f75,
  0xb29105af61e814fe, 0xc849756751dd9d87,
  0x2c31edf8f1d64ef6, 0x56e99d30c1e3c78f,
  0xd9810c6891bd5c04, 0xa3597ca0a188d57d,
  0xec09088b6997f879, 0x96d1784359a27100,
  0x19b9e91b09fcea8b, 0x636199d339c963f2,
  0xdf7adabd7a6e2d6f, 0xa5a2aa754a5ba416,
  0x2aca3b2d1a053f9d, 0x50124be52a30b6e4,
  0x1f423fcee22f9be0, 0x659a4f06d21a1299,
  0xeaf2de5e82448912, 0x902aae96b271006b,
  0x74523609127ad31a, 0x0e8a46c1224f5a63,
  0x81e2d7997211c1e8, 0xfb3aa75142244891,
  0xb46ad37a8a3b6595, 0xceb2a3b2ba0eecec,
  0x41da32eaea507767, 0x3b024222da65fe1e,
  0xa2722586f2d042ee, 0xd8aa554ec2e5cb97,
  0x57c2c41692bb501c, 0x2d1ab4dea28ed965,
  0x624ac0f56a91f461, 0x1892b03d5aa47d18,
  0x97fa21650afae693, 0xed2251ad3acf6fea,
  0x095ac9329ac4bc9b, 0x7382b9faaaf135e2,
  0xfcea28a2faafae69, 0x8632586aca9a2710,
  0xc9622c4102850a14, 0xb3ba5c8932b0836d,
  0x3cd2cdd162ee18e6, 0x460abd1952db919f,
  0x256b24ca6b12f26d, 0x5fb354025b277b14,
  0xd0dbc55a0b79e09f, 0xaa03b5923b4c69e6,
  0xe553c1b9f35344e2, 0x9f8bb171c366cd9b,
  0x10e3202993385610, 0x6a3b50e1a30ddf69,
  0x8e43c87e03060c18, 0xf49bb8b633338561,
  0x7bf329ee636d1eea, 0x012b592653589793,
  0x4e7b2d0d9b47ba97, 0x34a35dc5ab7233ee,
  0xbbcbcc9dfb2ca865, 0xc113bc55cb19211c,
  0x5863dbf1e3ac9dec, 0x22bbab39d3991495,
  0xadd33a6183c78f1e, 0xd70b4aa9b3f20667,
  0x985b3e827bed2b63, 0xe2834e4a4bd8a21a,
  0x6debdf121b863991, 0x1733afda2bb3b0e8,
  0xf34b37458bb86399, 0x8993478dbb8deae0,
  0x06fbd6d5ebd3716b, 0x7c23a61ddbe6f812,
  0x3373d23613f9d516, 0x49aba2fe23cc5c6f,
  0xc6c333a67392c7e4, 0xbc1b436e43a74e9d,
  0x95ac9329ac4bc9b5, 0xef74e3e19c7e40cc,
  0x601c72b9cc20db47, 0x1ac40271fc15523e,
  0x5594765a340a7f3a, 0x2f4c0692043ff643,
  0xa02497ca54616dc8, 0xdafce7026454e4b1,
  0x3e847f9dc45f37c0, 0x445c0f55f46abeb9,
  0xcb349e0da4342532, 0xb1eceec59401ac4b,
  0xfebc9aee5c1e814f, 0x8464ea266c2b0836,
  0x0b0c7b7e3c7593bd, 0x71d40bb60c401ac4,
  0xe8a46c1224f5a634, 0x927c1cda14c02f4d,
  0x1d148d82449eb4c6, 0x67ccfd4a74ab3dbf,
  0x289c8961bcb410bb, 0x5244f9a98c8199c2,
  0xdd2c68f1dcdf0249, 0xa7f41839ecea8b30,
  0x438c80a64ce15841, 0x3954f06e7cd4d138,
  0xb63c61362c8a4ab3, 0xcce411fe1cbfc3ca,
  0x83b465d5d4a0eece, 0xf96c151de49567b7,
  0x76048445b4cbfc3c, 0x0cdcf48d84fe7545,
  0x6fbd6d5ebd3716b7, 0x15651d968d029fce,
  0x9a0d8ccedd5c0445, 0xe0d5fc06ed698d3c,
  0xaf85882d2576a038, 0xd55df8e515432941,
  0x5a3569bd451db2ca, 0x20ed197575283bb3,
  0xc49581ead523e8c2, 0xbe4df122e51661bb,
  0x3125607ab548fa30, 0x4bfd10b2857d7349,
  0x04ad64994d625e4d, 0x7e7514517d57d734,
  0xf11d85092d094cbf, 0x8bc5f5c11d3cc5c6,
  0x12b5926535897936, 0x686de2ad05bcf04f,
  0xe70573f555e26bc4, 0x9ddd033d65d7e2bd,
  0xd28d7716adc8cfb9, 0xa85507de9dfd46c0,
  0x273d9686cda3dd4b, 0x5de5e64efd965432,
  0xb99d7ed15d9d8743, 0xc3450e196da80e3a,
  0x4c2d9f413df695b1, 0x36f5ef890dc31cc8,
  0x79a59ba2c5dc31cc, 0x037deb6af5e9b8b5,
  0x8c157a32a5b7233e, 0xf6cd0afa9582aa47,
  0x4ad64994d625e4da, 0x300e395ce6106da3,
  0xbf66a804b64ef628, 0xc5bed8cc867b7f51,
  0x8aeeace74e645255, 0xf036dc2f7e51db2c,
  0x7f5e4d772e0f40a7, 0x05863dbf1e3ac9de,
  0xe1fea520be311aaf, 0x9b26d5e88e0493d6,
  0x144e44b0de5a085d, 0x6e963478ee6f8124,
  0x21c640532670ac20, 0x5b1e309b16452559,
  0xd476a1c3461bbed2, 0xaeaed10b762e37ab,
  0x37deb6af5e9b8b5b, 0x4d06c6676eae0222,
  0xc26e573f3ef099a9, 0xb8b627f70ec510d0,
  0xf7e653dcc6da3dd4, 0x8d3e2314f6efb4ad,
  0x0256b24ca6b12f26, 0x788ec2849684a65f,
  0x9cf65a1b368f752e, 0xe62e2ad306bafc57,
  0x6946bb8b56e467dc, 0x139ecb4366d1eea5,
  0x5ccebf68aecec3a1, 0x2616cfa09efb4ad8,
  0xa97e5ef8cea5d153, 0xd3a62e30fe90582a,
  0xb0c7b7e3c7593bd8, 0xca1fc72bf76cb2a1,
  0x45775673a732292a, 0x3faf26bb9707a053,
  0x70ff52905f188d57, 0x0a2722586f2d042e,
  0x854fb3003f739fa5, 0xff97c3c80f4616dc,
  0x1bef5b57af4dc5ad, 0x61372b9f9f784cd4,
  0xee5fbac7cf26d75f, 0x9487ca0fff135e26,
  0xdbd7be24370c7322, 0xa10fceec0739fa5b,
  0x2e675fb4576761d0, 0x54bf2f7c6752e8a9,
  0xcdcf48d84fe75459, 0xb71738107fd2dd20,
  0x387fa9482f8c46ab, 0x42a7d9801fb9cfd2,
  0x0df7adabd7a6e2d6, 0x772fdd63e7936baf,
  0xf8474c3bb7cdf024, 0x829f3cf387f8795d,
  0x66e7a46c27f3aa2c, 0x1c3fd4a417c62355,
  0x935745fc4798b8de, 0xe98f353477ad31a7,
  0xa6df411fbfb21ca3, 0xdc0731d78f8795da,
  0x536fa08fdfd90e51, 0x29b7d047efec8728,
};

DECLSPEC u32x round_crc32 (u32x a, const u32x v, CONSTANT_AS u32 *crc32table)
{
  const u32x k = (a ^ v) & 0xff;

  const u32x s = a >> 8;

  #if   VECT_SIZE == 1
  a = make_u32x crc32table[k];
  #elif VECT_SIZE == 2
  a = make_u32x (crc32table[k.s0], crc32table[k.s1]);
  #elif VECT_SIZE == 4
  a = make_u32x (crc32table[k.s0], crc32table[k.s1], crc32table[k.s2], crc32table[k.s3]);
  #elif VECT_SIZE == 8
  a = make_u32x (crc32table[k.s0], crc32table[k.s1], crc32table[k.s2], crc32table[k.s3], crc32table[k.s4], crc32table[k.s5], crc32table[k.s6], crc32table[k.s7]);
  #elif VECT_SIZE == 16
  a = make_u32x (crc32table[k.s0], crc32table[k.s1], crc32table[k.s2], crc32table[k.s3], crc32table[k.s4], crc32table[k.s5], crc32table[k.s6], crc32table[k.s7], crc32table[k.s8], crc32table[k.s9], crc32table[k.sa], crc32table[k.sb], crc32table[k.sc], crc32table[k.sd], crc32table[k.se], crc32table[k.sf]);
  #endif

  a ^= s;

  return a;
}

DECLSPEC u32 round_crc32_l_S (const u32 a, const u32 v, LOCAL_AS u32 *crc32table)
{
  const u32 k = (a ^ v) & 0xff;

  const u32 s = a >> 8;

  return crc32table[k] ^ s;
}

DECLSPEC u32 round_crc32_16_S (const u32 crc32, PRIVATE_AS const u32 *buf, const u32 len, LOCAL_AS u32 *crc32table)
{
  #define MIN(a,b) (((a) < (b)) ? (a) : (b))

  const int crc_len = MIN (len, 16);

  #undef MIN

  u32 c = crc32;

  for (int i = 0; i < crc_len; i++)
  {
    const u32 idx = i / 4;
    const u32 mod = i % 4;
    const u32 sht = (3 - mod) * 8;

    const u32 b = buf[idx] >> sht; // b & 0xff (but already done in round_crc32 ())

    c = round_crc32_l_S (c, b, crc32table);
  }

  return c;
}

DECLSPEC u64 round_crc64_S (u64 a, const u64 v, SHM_TYPE u64 *crc64table)
{
  const u64 k = (a ^ v) & 0xff;

  const u64 s = a >> 8;

  a = crc64table[k];

  a ^= s;

  return a;
}

DECLSPEC u32 round_crc32_S (u32 a, const u32 v, CONSTANT_AS u32 *crc32table)
{
  const u32 k = (a ^ v) & 0xff;

  const u32 s = a >> 8;

  a = crc32table[k];

  a ^= s;

  return a;
}

DECLSPEC u32 round_crc32_4_S (const u32 w, const u32 iv, CONSTANT_AS u32 *crc32table)
{
  u32 a = iv;

  a = round_crc32_S (a, w >>  0, crc32table);
  a = round_crc32_S (a, w >>  8, crc32table);
  a = round_crc32_S (a, w >> 16, crc32table);
  a = round_crc32_S (a, w >> 24, crc32table);

  return a;
}

DECLSPEC u64 crc64j_opti (PRIVATE_AS const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table)
{
  u64 a = iv;

  if (pw_len >=  1) a = round_crc64_S (a, w[0] >>  0, crc64table);
  if (pw_len >=  2) a = round_crc64_S (a, w[0] >>  8, crc64table);
  if (pw_len >=  3) a = round_crc64_S (a, w[0] >> 16, crc64table);
  if (pw_len >=  4) a = round_crc64_S (a, w[0] >> 24, crc64table);
  if (pw_len >=  5) a = round_crc64_S (a, w[1] >>  0, crc64table);
  if (pw_len >=  6) a = round_crc64_S (a, w[1] >>  8, crc64table);
  if (pw_len >=  7) a = round_crc64_S (a, w[1] >> 16, crc64table);
  if (pw_len >=  8) a = round_crc64_S (a, w[1] >> 24, crc64table);
  if (pw_len >=  9) a = round_crc64_S (a, w[2] >>  0, crc64table);
  if (pw_len >= 10) a = round_crc64_S (a, w[2] >>  8, crc64table);
  if (pw_len >= 11) a = round_crc64_S (a, w[2] >> 16, crc64table);
  if (pw_len >= 12) a = round_crc64_S (a, w[2] >> 24, crc64table);

  for (u32 i = 12, j = 3; i < pw_len; i += 4, j += 1)
  {
    if (pw_len >= (i + 1)) a = round_crc64_S (a, w[j] >>  0, crc64table);
    if (pw_len >= (i + 2)) a = round_crc64_S (a, w[j] >>  8, crc64table);
    if (pw_len >= (i + 3)) a = round_crc64_S (a, w[j] >> 16, crc64table);
    if (pw_len >= (i + 4)) a = round_crc64_S (a, w[j] >> 24, crc64table);
  }

  return a;
}

DECLSPEC u32x crc32_opti (PRIVATE_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  if (pw_len >=  1) a = round_crc32 (a, w[0] >>  0, crc32tab);
  if (pw_len >=  2) a = round_crc32 (a, w[0] >>  8, crc32tab);
  if (pw_len >=  3) a = round_crc32 (a, w[0] >> 16, crc32tab);
  if (pw_len >=  4) a = round_crc32 (a, w[0] >> 24, crc32tab);
  if (pw_len >=  5) a = round_crc32 (a, w[1] >>  0, crc32tab);
  if (pw_len >=  6) a = round_crc32 (a, w[1] >>  8, crc32tab);
  if (pw_len >=  7) a = round_crc32 (a, w[1] >> 16, crc32tab);
  if (pw_len >=  8) a = round_crc32 (a, w[1] >> 24, crc32tab);
  if (pw_len >=  9) a = round_crc32 (a, w[2] >>  0, crc32tab);
  if (pw_len >= 10) a = round_crc32 (a, w[2] >>  8, crc32tab);
  if (pw_len >= 11) a = round_crc32 (a, w[2] >> 16, crc32tab);
  if (pw_len >= 12) a = round_crc32 (a, w[2] >> 24, crc32tab);

  for (u32 i = 12, j = 3; i < pw_len; i += 4, j += 1)
  {
    if (pw_len >= (i + 1)) a = round_crc32 (a, w[j] >>  0, crc32tab);
    if (pw_len >= (i + 2)) a = round_crc32 (a, w[j] >>  8, crc32tab);
    if (pw_len >= (i + 3)) a = round_crc32 (a, w[j] >> 16, crc32tab);
    if (pw_len >= (i + 4)) a = round_crc32 (a, w[j] >> 24, crc32tab);
  }

  return ~a;
}

DECLSPEC u32x crc32c_opti (PRIVATE_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  if (pw_len >=  1) a = round_crc32 (a, w[0] >>  0, crc32ctab);
  if (pw_len >=  2) a = round_crc32 (a, w[0] >>  8, crc32ctab);
  if (pw_len >=  3) a = round_crc32 (a, w[0] >> 16, crc32ctab);
  if (pw_len >=  4) a = round_crc32 (a, w[0] >> 24, crc32ctab);
  if (pw_len >=  5) a = round_crc32 (a, w[1] >>  0, crc32ctab);
  if (pw_len >=  6) a = round_crc32 (a, w[1] >>  8, crc32ctab);
  if (pw_len >=  7) a = round_crc32 (a, w[1] >> 16, crc32ctab);
  if (pw_len >=  8) a = round_crc32 (a, w[1] >> 24, crc32ctab);
  if (pw_len >=  9) a = round_crc32 (a, w[2] >>  0, crc32ctab);
  if (pw_len >= 10) a = round_crc32 (a, w[2] >>  8, crc32ctab);
  if (pw_len >= 11) a = round_crc32 (a, w[2] >> 16, crc32ctab);
  if (pw_len >= 12) a = round_crc32 (a, w[2] >> 24, crc32ctab);

  for (u32 i = 12, j = 3; i < pw_len; i += 4, j += 1)
  {
    if (pw_len >= (i + 1)) a = round_crc32 (a, w[j] >>  0, crc32ctab);
    if (pw_len >= (i + 2)) a = round_crc32 (a, w[j] >>  8, crc32ctab);
    if (pw_len >= (i + 3)) a = round_crc32 (a, w[j] >> 16, crc32ctab);
    if (pw_len >= (i + 4)) a = round_crc32 (a, w[j] >> 24, crc32ctab);
  }

  return ~a;
}

DECLSPEC u32x crc32 (PRIVATE_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  PRIVATE_AS const u8 *w_ptr = (PRIVATE_AS const u8 *) w;

  for (u32 i = 0; i < pw_len; i++)
  {
    const u32 v = (const u32) w_ptr[i];

    a = round_crc32 (a, v, crc32tab);
  }

  return ~a;
}

DECLSPEC u32x crc32_global (GLOBAL_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  GLOBAL_AS const u8 *w_ptr = (GLOBAL_AS const u8 *) w;

  for (u32 i = 0; i < pw_len; i++)
  {
    const u32 v = (const u32) w_ptr[i];

    a = round_crc32 (a, v, crc32tab);
  }

  return ~a;
}

DECLSPEC u32x crc32c (PRIVATE_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  PRIVATE_AS const u8x *w_ptr = (PRIVATE_AS const u8x *) w;

  for (u32 i = 0; i < pw_len; i++)
  {
    const u32x v = (const u32x) w_ptr[i];

    a = round_crc32 (a, v, crc32ctab);
  }

  return ~a;
}

DECLSPEC u32x crc32c_global (GLOBAL_AS const u32x *w, const u32 pw_len, const u32 iv)
{
  u32x a = ~iv;

  GLOBAL_AS const u8x *w_ptr = (GLOBAL_AS const u8x *) w;

  for (u32 i = 0; i < pw_len; i++)
  {
    const u32x v = (const u32x) w_ptr[i];

    a = round_crc32 (a, v, crc32ctab);
  }

  return ~a;
}

DECLSPEC u64 crc64j (PRIVATE_AS const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table)
{
  u64 a = iv;

  PRIVATE_AS const u8 *w_ptr = (PRIVATE_AS const u8 *) w;

  for (int i = 0; i < pw_len; i++)
  {
    const u64 v = (const u64) w_ptr[i];

    a = round_crc64_S (a, v, crc64table);
  }

  return a;
}

DECLSPEC u64 crc64j_global (GLOBAL_AS const u32 *w, const u32 pw_len, const u64 iv, SHM_TYPE u64 *crc64table)
{
  u64 a = iv;

  GLOBAL_AS const u8 *w_ptr = (GLOBAL_AS const u8 *) w;

  for (int i = 0; i < pw_len; i++)
  {
    const u64 v = (const u64) w_ptr[i];

    a = round_crc64_S (a, v, crc64table);
  }

  return a;
}
