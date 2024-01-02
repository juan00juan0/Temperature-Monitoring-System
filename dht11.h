#ifndef DHT11_H
#define DHT11_H

void startDHT() {
    TRISC7 = 0;
    
    LATC7 = 0;
    __delay_ms(18);
    
    LATC7 = 1;
    __delay_us(20);
    
    TRISC7 = 1;
}

unsigned char responseDHT() {
    unsigned char check_bit = 0;
    
    __delay_us(40);
    
    if (RC7 == 0){
        
        __delay_us(80);
    
        if (RC7 == 1){
            
            check_bit = 1;
            while(RC7);
            
        }
    }
    
    return check_bit;
}

unsigned char readDHT() {
    char data, for_count;
    
    for(for_count = 0; for_count < 8; for_count++){
        while(!RC7);
        __delay_us(26);
        
        if(RC7 == 0){
            
            data &= ~(1 << (7 - for_count)); //Clear bit (7-b)
            
        }
        else{
            
            data |= (1 << (7 - for_count)); //Set bit (7-b)
            
            while(RC7);
            
        }
    }
    
    return data;
}

#endif