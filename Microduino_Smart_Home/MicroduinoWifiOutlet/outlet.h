
void doOutlet()
{
    //print currentInfo
  if(currentInfo!=NULL) {
    for (int i = 0; i < currentInfo.length(); i++) {
      if (currentInfo.substring(i, i+1) == ":") {
        outletState = (currentInfo.substring(0, i)=="true") ? true : false;
        break;
      }
    }

    if(outletState) {
      digitalWrite(outletPin,HIGH);
    } else {
      digitalWrite(outletPin,LOW);
    }
  }
}
