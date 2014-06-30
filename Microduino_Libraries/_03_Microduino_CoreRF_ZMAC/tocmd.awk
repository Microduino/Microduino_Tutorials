BEGIN {
  print "set MAC_INSTALL=C:\\Atmel\\MAC_v_2_6_1"
  print "set GAWK=gawk.exe" 
}
/^GAWK=/ { next }
/^MAC_INSTALL=/ { next }
/^[A-Z_]*=/ {
  print "set",gensub(/[$]([A-Z_]*)/,"%\\1%","g",$0); 
  next
}
{
  print gensub(/[$]([A-Z_]*)/,"%\\1%","g",$0); 
}
