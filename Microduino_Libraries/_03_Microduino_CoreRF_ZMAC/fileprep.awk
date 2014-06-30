#
# Gawk script to modify MAC source files to work in Arduino univers
# 

BEGIN {

#
# Arduino does not let us pass in -DXXX=YYY on the command line, so all
# the C files need to get the -D defines crammed in at the top
#

  print "#include <ZMAC.h>"
}
/^#define AVR/ {
  print "#undef AVR"
}

/^} app_timer_t\;/ {

  print ",APP_TIMER_INDIRECT_DATA = (APP_FIRST_TIMER_ID),"
  print "APP_TIMER_POLL_DATA = (APP_FIRST_TIMER_ID),"
  print "APP_TIMER_LED_OFF = (APP_FIRST_TIMER_ID + 1)"
}

{
#
# pal.h tries to redefine HIGH and LOW, which is a problem because
# the core arduino defines them too
#
  gsub(/== HIGH/,"== pin_HIGH")
  gsub(/= LOW/,"= pin_LOW")
  gsub(/= HIGH/,"= pin_HIGH")
  gsub(/^    HIGH/,"    pin_HIGH")
  gsub(/^    LOW,/,"    pin_LOW,")
  
  gsub(/main.void/,"example_main(void")

#
# Work around the one c99 usage they like, "for (int x=1..." by
# reordering the for expressions.  Would be better to get
# -std=c99 on the command line!
#
  $0 = gensub(/(for ?\()(\w+ )(\w+)/,"\\2\\3 ; \\1\\3","g",$0)

#
# After all that, print the line back out
#
  print $0
}
