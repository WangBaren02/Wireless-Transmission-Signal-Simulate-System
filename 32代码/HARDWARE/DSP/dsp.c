#include "dsp.h"

u32 max_min(u32 *buf,u16 num,u16 start,u8 k) //buf输入数组，num处理个数，k = 0 ->max
{
	u32 res = 0;
	u16 i;
	if(k == 0)
	{
		res = 0;
		for(i = 0;i<num;i++)
		{
			if(res<buf[i + start])
			res = buf[i + start];
		}
		return res;
	}
	else
	{
		res = 0x7fff;
		for(i = 0;i<num;i++)
		{
			if(res>buf[i + start])
			res = buf[i + start];
		}
		return res;
	}
}

u8 mode_decide(u32 *buf,u32 space)
{
	int flag=0;
	u32 max1,max2,max3,max4;
	u32 poi1,poi2,poi3,poi4;
	max1 = buf[1];
	max2 = buf[3];
	max3 = buf[5];
	max4 = buf[7];
	poi1 = buf[0];
	poi2 = buf[2];
	poi3 = buf[4];
	poi4 = buf[6];
	
	
//	if((max2<0.05*max1)&&(max3<0.05*max1)&&(max4<0.05*max1))
//	  return 0;
//	if((max2>0.1*max1)&&(max3>0.1*max1)&&(max4<0.05*max1))
//		return 1;
//	if(((max2-max3)<0.1*max1)&&(max2<0.8*max1)&&(max4<0.25*max1))
//		return 2;
//	if(((max1-max2)<0.14*max1)&&((max3-max4)<0.14*max3)&&(max3<0.5*max1))
//		return 3;
//	if(space == 5||space == 10||space == 15||space == 20||space == 25)
//		return 5;
//	else
//		return 4;
	if(space==43||space==42)space=50;
	if(space==23||space==22)space=40;
	if(space==16)space=30;
	
	if(space<=26&&space>=4)flag=1;
	else if (space>=29&&space<120)flag=2;
	if(flag==1&&(max2<0.55*max1)&&(max3<0.55*max1)&&(max4<0.15*max1))return 1;
	else if(flag==1)return 5;
	if(flag==2&&((max1-max2)<0.14*max1)&&((max3-max4)<0.14*max3)&&(max3<0.5*max1))return 3;
	else if(flag==2&&((max2-max3)<0.14*max1)&&(max2<0.8*max1)&&(max4<0.25*max1))return 2;
	else if(flag==2)return 4;
	return 0;
}

