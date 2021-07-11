#ifndef common_INCLUDED
#define common_INCLUDED


int set_RST(int cport_nr);

/*Clear event register*/
int set_CLS(int cport_nr);

/*Set ESE register*/
int set_ESE(int cport_nr, int bit);

/*Set SRE register*/
int set_SRE(int cport_nr, int bit);

#endif
