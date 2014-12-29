macro movntq mdst, msrc {
  if mdst in <eax,ebx,ecx,edx,esi,edi,ebp,ax,bx,cx,dx,si,di,bp> & msrc = 0
    xor  mdst, mdst
  else
    mov  mdst, msrc
  end if
}

macro cmpntq minp, mref {
  if minp in <bl,cl,dl,ah,bh,ch,dh,ax,bx,cx,dx,si,di,bp,\
	      eax,ebx,ecx,edx,esi,edi,ebp> & mref = 0
    test minp, minp
  else
    cmp  minp, mref
  end if
}

macro ALIGN alignment{
  mod_result = ($ mod 16)
  if (mod_result > 0)
    times mod_result db 0
  end if
}