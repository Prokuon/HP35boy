void loop()
{
    prevCarry = carry;
    carry = 0;
    fetch_h = 0;
    fetch_l = 0;
    fetch_h = pgm_read_byte_near ((enable_bug?rom_bug:rom) + ( offset * 256 * 2 ) + (pc * 2)    );
    fetch_l = pgm_read_byte_near ((enable_bug?rom_bug:rom) + ( offset * 256 * 2 ) + (pc * 2) + 1 );

    if ((pc == 191) & (offset == 0)) {
        error = 1;
    }

    if (error == 1) {
        Timer1.start();
    }


    pc++;
    pc %= 256;

    //ulteriore elaborazione del tasto ricevuto
    if (key_code < 255) {
        error = 0;
        Timer1.stop();
        digitalWrite(RED_LED, HIGH);

        key_rom = key_code;

        key_code = -1;
        s[0] = 1;
    }

    //Classifico le istruzioni per tipo

    if (fetch_l == 0x00) {

        //NOP
    }

    if ((fetch_l & 0x03) == 0x01) {
        //Jump subroutine


        ret = pc;
        pc = (((fetch_l >> 2) & 0x3f) | ((fetch_h << 6) & 0x0c0));

    }

    if ((fetch_l & 0x7F) == 0x030) {
        //("Return from Subroutine");

        pc = ret;
    }

    if ((fetch_l & 0x7F) == 0x010) {
        // ("Select ROM");

        offset = (       ((fetch_h << 1) & 06)   |    ((fetch_l >> 7) & 01)      );

    }

    //testa se Ã¨ diponibile un carattere da tastiera
    //se si lo inserisce nel buffer e setta ad 1 la variabile key
    //la cpu fa un salto incondizionato all'indirizzo pc+rom (istruzione keyrom)
    if (fetch_l == 0x0D0) {
        pc = key_rom;
        //resetta s0 non controlla se era ad 1 prima
        s[0] = 0;
        // ("KEYROM");

    }

    //routine di servizio che legge i dati in input
    //key_code = -1;


    //*********************************************************

    if ((fetch_l & 0x03f) == 0x014) {
        //testa il bit n dello status( copia il bit nel carry)
        // ("Test bit n dello Status ");

        carry = s[ ((fetch_h & 0x03) << 2)  | ( (fetch_l & 0x0c0) >> 6) ];
    }


    if ((fetch_l & 0x03f) == 0x004) {
        //setta il bit n dello status
        // ("Set bit n dello Status ");

        s[ ((fetch_h & 0x03) << 2)  | ( (fetch_l & 0x0c0) >> 6) ] = 1;
    }

    if ((fetch_l & 0x03f) == 0x024) {
        //resetta il bit n dello status
        // ("ReSet bit n dello Status ");

        s[ ((fetch_h & 0x03) << 2)  | ( (fetch_l & 0x0c0) >> 6) ] = 0;
    }

    if ((fetch_l & 0x03f) == 0x034) {
        //resetta tutti i bit dello status
        // ("Cancella tutti bit dello Status ");
        for (i = 0; i <= 12; i++) s[i] = 0;
    }

    //*********************************************************



    //*********************************************************

    if ((fetch_l & 0x03f) == 0x02C) {
        //testa il bit n dello status( copia il bit nel carry)
        // ("Test se P=numero N carry = 1 ");

        carry = 0;
        if (p == (((fetch_h & 0x03) << 2)  | ( (fetch_l & 0x0c0) >> 6)) ) carry = 1;
    }


    if ((fetch_l & 0x03f) == 0x00C) {
        //setta il bit n dello status
        //("Set Pointer con n ");

        p = (((fetch_h & 0x03) << 2)  | ( (fetch_l & 0x0c0) >> 6) );
    }

    if ((fetch_l & 0x03f) == 0x03C) {
        //resetta il bit n dello status
        //("Incrementa P ");

        p = ((p + 1) & 0x0f);
    }

    if ((fetch_l & 0x03f) == 0x01C) {
        //resetta tutti i bit dello status
        //("Decrementa P ");
        p = ((p - 1) & 0x0f);
    }
    //*********************************************************

    //*********************************math operation
    //*******************************************************

    if ((fetch_l & 0x3F) == 0x18) {
        //Data entry - Display
        //("Load Costant");
        //function loadconst(num) { return function () { c[p] = num; p = (p - 1) & 0xf; }; }
        c[p] = ((fetch_l >> 6) | (fetch_h << 2));
        p = ( (p - 1) & 0x0f);
    }


    if (((fetch_h & 0x03) == 0x00) && ((fetch_l & 0x0ef) == 0x0A8)) {
        //exch_m
        for (i = 0; i < WSIZE; i++)

        {

        tm = c [i]; c [i] = m[i]; m [i] = tm;

        }
    }
    if (((fetch_h & 0x03) == 0x01) && ((fetch_l & 0x0ef) == 0x028)) {
        //c_to_stack
        for (i = 0; i < WSIZE; i++)

        {

        f [i] = e [i];

        e [i] = d [i];

        d [i] = c [i];

        }

    }
    if (((fetch_h & 0x03) == 0x01) && ((fetch_l & 0x0ef) == 0x0A8)) {
        //stack_to_a
        for (i = 0; i < WSIZE; i++)

        {

        a [i] = d [i];

        d [i] = e [i];

        e [i] = f [i];

        }
    }
    if (((fetch_h & 0x03) == 0x02) && ((fetch_l & 0x0ef) == 0x0A8)) {
        //m_to_c
        for (i = 0; i < WSIZE; i++)

        c [i] = m [i];
    }
    if (((fetch_h & 0x03) == 0x03) && ((fetch_l & 0x0ef) == 0x028)) {
        //down_rotate
        for (i = 0; i < WSIZE; i++)

        {

        tm = c [i];

        c [i] = d [i];

        d [i] = e [i];

        e [i] = f [i];

        f [i] = tm;

        }
    }
    if (((fetch_h & 0x03) == 0x03) && ((fetch_l & 0x0ef) == 0x0A8)) {
        //clear_reg
        for (i = 0; i < WSIZE; i++)
        a [i] = b [i] = c [i] = d [i] = e [i] = f [i] = m [i] = 0;

    }



    if (((fetch_h & 0x03) == 0x00) && ((fetch_l & 0x0ef) == 0x028)) {
        display_enable = false;
        update_display =  false;

    }



    if (((fetch_h & 0x03) == 0x02) && ((fetch_l & 0x0ef) == 0x028)) { //display toggle
        display_enable = ! display_enable;
        if (display_enable == true) {
        update_display = true;

        }
    }

    if ((fetch_l & 0x03) == 0x03) {
        //salto condizionale
        //("Conditional Branch --> ");




        if ( prevCarry != 1) {
        pc = ((fetch_l & 0x0fc) >> 2)  | ( (fetch_h & 0x03) << 6);
        }
    }

    if ((fetch_l & 0x03) == 0x02) {
        //A&R
        word_select = ((fetch_l >> 2) & 0x07);
        op_code = ((fetch_l >> 5) & 0x07);
        op_code = op_code | ((fetch_h << 3) & 0x018);

        get_f_l(word_select);

        switch (op_code) {

        case 0x0:
            //("A&R 0+B");
            carry = 0; //
            for (i = first; i <= last; i++)
            carry |= (b[i] != 0) ? 1 : 0;
            break;

        case 0x01:
            //("A&R 0->B");

            for (i = first; i <= last; i++) {
            b[i] = 0;
            }
            carry = 0;
            break;

        case 0x02:
            // ("A&R A-C");
            carry = 0;
            for (i = first; i <= last; i++)
            t[i] = do_sub (a[i], c[i]);
            break;

        case 0x03:
            //  ("A&R C-1");
            carry = 1;
            for (i = first; i <= last; i++)
            carry &= (c[i] == 0) ? 1 : 0;
            break;



        case 0x04:
            //("A&R B->C");

            for (i = first; i <= last; i++) {
            c[i] = b[i];
            }
            carry = 0;
            break;

        case 0x05:
            //   ("A&R 0-C->C");
            carry = 0;
            for (i = first; i <= last; i++)
            c[i] = do_sub (zero, c[i]);

            break;


        case 0x06:
            //("A&R 0->C");

            for (i = first; i <= last; i++) {
            c[i] = 0;
            }
            carry = 0;
            break;

        case 0x07:
            //  ("A&R 0-C-1->C");
            carry = 1;
            for (i = first; i <= last; i++)
            c[i] = do_sub (zero, c[i]);
            break;

        case 0x08:
            //  ("A&R Sh A Left");
            for (i = last; i >= first; i--)
            a[i] = (i == first) ? 0 : a [i - 1];
            carry = 0;
            break;

        case 0x09:
            //("A&R A->B");

            for (i = first; i <= last; i++) {
            b[i] = a[i];
            }
            carry = 0;
            break;

        case 0x0a:
            //("A&R A-C->C");
            carry = 0;
            for (i = first; i <= last; i++)
            c [i] = do_sub (a [i], c [i]);
            break;

        case 0x0b:
            //   ("A&R C-1->C");
            carry = 1;
            for (i = first; i <= last; i++)
            c[i] = do_sub (c[i], zero);
            break;

        case 0x0c:
            //("A&R C->A");

            for (i = first; i <= last; i++) {
            a[i] = c[i];
            }
            carry = 0;
            break;

        case 0x0d:
            //  ("A&R 0-C");//influenza solo i flag


            for (i = first; i <= last; i++)
            carry |= (c[i] != 0) ? 1 : 0;
            break;


        case 0x0e:
            // ("A&R A+C->C");
            carry = 0;
            for (i = first; i <= last; i++)
            c[i] = do_add (a[i], c[i]);

            break;

        case 0x0f:
            //    ("A&R C+1->C");
            carry = 1;
            for (i = first; i <= last; i++)
            c[i] = do_add (c[i], zero);
            break;

        case 0x010:
            //  ("A&R A-B");

            for (i = first; i <= last; i++)
            t[i] = do_sub (a[i], b[i]);
            break;

        case 0x011:
            // ("A&R B<->C");

            for (i = first; i <= last; i++) {
            temp = b[i];
            b[i] = c[i];
            c[i] = temp;
            }
            carry = 0;
            break;

        case 0x012:
            //    ("A&R Sh C Right");
            for (i = first; i <= last; i++)
            c[i] = (i == last) ? 0 : c [i + 1];
            carry = 0;
            break;

        case 0x013:
            //  ("A&R A-1");
            carry = 1;
            for (i = first; i <= last; i++)
            carry &= (a[i] == 0) ? 1 : 0;
            break;

        case 0x014:
            //  ("A&R Sh B Right");
            for (i = first; i <= last; i++)
            b[i] = (i == last) ? 0 : b [i + 1];
            carry = 0;
            break;

        case 0x015:
            // ("A&R C+C->A");
            carry = 0;
            for (i = first; i <= last; i++)
            c[i] = do_add (c[i], c[i]);
            break;


        case 0x016:
            //   ("A&R Sh A Right");
            for (i = first; i <= last; i++)
            a[i] = (i == last) ? 0 : a [i + 1];
            carry = 0;
            break;


        case 0x017:

            //("A&R 0->A");//ok

            for (i = first; i <= last; i++) {
            a[i] = 0;
            }
            carry = 0;
            break;

        case 0x018:
            // ("A&R A-B->A");
            carry = 0;
            for (i = first; i <= last; i++)
            a[i] = do_sub (a[i], b[i]);
            break;

        case 0x019:
            //("A&R A<->B");

            for (i = first; i <= last; i++) {
            temp = a[i];
            a[i] = b[i];
            b[i] = temp;
            }
            carry = 0;
            break;

        case 0x01a:
            // ("A&R A-C->A");

            carry = 0;
            for (i = first; i <= last; i++)
            a[i] = do_sub (a[i], c[i]);
            break;

        case 0x01b:
            //  ("A&R A-1->A");

            carry = 1;
            for (i = first; i <= last; i++)
            a[i] = do_sub (a[i], zero);
            break;

        case 0x01c:
            //("A&R A+B->A");

            carry = 0;
            for (i = first; i <= last; i++)
            a[i] = do_add (a[i], b[i]);
            break;

        case 0x01d:
            //("A&R C<->A");

            for (i = first; i <= last; i++) {
            temp = a[i];
            a[i] = c[i];
            c[i] = temp;
            }
            carry = 0;
            break;

        case 0x01e:
            // ("A&R A+C->A");

            carry = 0;
            for (i = first; i <= last; i++)
            a[i] = do_add (a[i], c[i]);
            break;

        case 0x01f:
            //  ("A&R A+1->A");

            carry = 1;
            for (i = first; i <= last; i++)
            a[i] = do_add (a[i], zero);
            break;
        }
        //A&R fine

    }

    //**************************************************************************

    if (display_enable) {

        update_display = true;
    }
    if (!display_enable) {

        if (update_display) {
        lcd_HP35();

        update_display = false;

        }
    }

    //**************************************************************************
    arduboy.pollButtons();
    if (arduboy.justReleased(LEFT_BUTTON))
    {
        currentKey = keyboardSelectByPosition(2);
        update_display = true;
    }
    if (arduboy.justReleased(UP_BUTTON))
    {
        currentKey = keyboardSelectByPosition(3);
        update_display = true;
    }
    if (arduboy.justReleased(RIGHT_BUTTON))
    {
        currentKey = keyboardSelectByPosition(0);
        update_display = true;
    }
    if (arduboy.justReleased(DOWN_BUTTON))
    {
        currentKey = keyboardSelectByPosition(1);
        update_display = true;
    }
    if (arduboy.justReleased(A_BUTTON))
        key_code = keyboard[currentKey].keycode;

    if (arduboy.justReleased(B_BUTTON))
    {
        byte c = keyboardSelectByCode(62);
        if(currentKey == c)
        currentKey = anotherButton; 
        else
        {
        anotherButton = currentKey;
        currentKey = c;
        }
        update_display = true;
    }

}//fine ciclo loop
