/* 
 * Update GPU execution queue. Return number of positions in current queue.
*/
int updateQue (void)
{
    // Return 1 immediately, if DMA transfer is still busy.
    if (*gpu_dma_chcr & 0x01000000) return 1;

    // Ignore interrupts.
    genv.oldmask = SetIntrMask (0); 

    // Execute queue entries, until GPU DMA is not busy.
    while ( !(*gpu_dma_chcr & 0x01000000) && (_qout != _qin) )
    {
        if ( ((_qout + 1) & 0x3F) == _qin )
        {
            if (data_5C == NULL) DMACallback (2, NULL);
        }

        // Wait until GPU become busy.
        if ( *gp1 & 0x04000000 )
        {
            while ( !(*gp1 & 0x04000000) ) ;
        }

        _que[_qout].cb (_que[_qout].p1, _que[_qout].p2);

        _qout = (_qout + 1) & 0x3F;
    }

    // Allow interrupts generation.
    SetIntrMask (genv.oldmask);

    if (data_5C && data_58)
    {
        data_58 = 0;
        data_5C ();
    }

    return  (_qin - _qout) & 0x3F;
}
