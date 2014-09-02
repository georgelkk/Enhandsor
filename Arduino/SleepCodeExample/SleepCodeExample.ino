set_sleep_mode(SLEEP_MODE_PWR_DOWN );   
  sleep_enable();

  // http://forum.arduino.cc/index.php/topic,4087.0.html
 
  //23.8.2 ADCSRA – ADC Control and Status Register A
  //Bit 7 – ADEN: ADC Enable
  //Writing this bit to one enables the ADC. By writing it to zero, the ADC is turned off. Turning the
  //ADC off while a conversion is in progress, will terminate this conversion.

 //So this should shut off the converter...
  ADCSRA &= ~(1 << ADEN);
  // ADCSRA = ADCSRA & ~(1^(ADEN))
  
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();

