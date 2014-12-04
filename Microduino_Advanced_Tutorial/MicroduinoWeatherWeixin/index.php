<?php

$mydatabase="app_leirobot";

//错误日志
function echo_server_log($log){
	file_put_contents("log.txt", $log, FILE_APPEND);
}
//定义TOKEN
define ( "TOKEN", "ulink" );
//验证微信公众平台签名
function checkSignature() {
	$signature = $_GET ['signature'];
	$nonce = $_GET ['nonce'];
	$timestamp = $_GET ['timestamp'];
	$tmpArr = array ($nonce, $timestamp, TOKEN );
	sort ( $tmpArr );
	$tmpStr = implode ( $tmpArr );
	$tmpStr = sha1 ( $tmpStr );
	if ($tmpStr == $signature) {
		return true;
	}else{
		return false;
	}
}
if(false == checkSignature()) {
	exit(0);
}
//接入时验证接口
$echostr = $_GET ['echostr'];
if($echostr) {
	echo $echostr;
	exit(0);
}
//获取POST数据
function getPostData() {
	$data = $GLOBALS['HTTP_RAW_POST_DATA'];
	return	$data;
}
$PostData = getPostData();
//验错
if(!$PostData){
	echo_server_log("wrong input! PostData is NULL");
	echo "wrong input!";
	exit(0);
}
//装入XML
$xmlObj = simplexml_load_string($PostData, 'SimpleXMLElement', LIBXML_NOCDATA);
//验错
if(!$xmlObj) {
	echo_server_log("wrong input! xmlObj is NULL\n");
	echo "wrong input!";
	exit(0);
}
//准备XML
$fromUserName = $xmlObj->FromUserName;
$toUserName = $xmlObj->ToUserName;
$msgType = $xmlObj->MsgType;
if($msgType == 'voice') {//判断是否为语音
	$content = $xmlObj->Recognition;
}elseif($msgType == 'text'){
	$content = $xmlObj->Content;
}else{
	$retMsg = '只支持文本和语音消息';
}

try {

$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS);

mysql_select_db($mydatabase, $con);//修改数据库名

//判断指令
if (strstr($content, "hh")) {

	$result = mysql_query("SELECT * FROM sensor");
	while($arr = mysql_fetch_array($result)){
		switch ($arr['ID'])
		{
		case 1:
		  $retMsg.="温度:".$arr['data']." ℃"."\n";
			break;  
		case 2:
		  $retMsg.="湿度:".$arr['data']." %"."\n";
			break;
		case 3:
		  $retMsg.="光照:".$arr['data']." lux"."\n";
			break;
		case 4:
		  $retMsg.="气压:".$arr['data']." kPa"."\n";
			break;
		default:
			break;
		}
	}

	$retMsg .= "Microduino suport";

} else {

	$avalible=false;

	if (strstr($content, "开灯")) {
		$state='1';
		$avalible=true;
	} 
	if (strstr($content, "关灯")) {
		$state='0';
		$avalible=true;
	}

	if($avalible) {
		$state='0';
		$dati = date("h:i:sa");
		$sql ="UPDATE switch SET timestamp='$dati',state = $state
		WHERE ID = '1'";//修改开关状态值
		if(!mysql_query($sql,$con)){
			die('Error: ' . mysql_error());
		}else{
			$retMsg = "好的主人";
		}
	} else {
		$retMsg = "暂时不支持该命令";
	}
}

mysql_close($con);

} catch(Exception $e) {
	print $e->getMessage(); 
	exit();
}

//装备XML
$retTmp = "<xml>
<ToUserName><![CDATA[%s]]></ToUserName>
<FromUserName><![CDATA[%s]]></FromUserName>
<CreateTime>%s</CreateTime>
<MsgType><![CDATA[text]]></MsgType>
<Content><![CDATA[%s]]></Content>
<FuncFlag>0</FuncFlag>
</xml>";
$resultStr = sprintf($retTmp, $fromUserName, $toUserName, time(), $retMsg);
//反馈到微信服务器
echo $resultStr;
?>