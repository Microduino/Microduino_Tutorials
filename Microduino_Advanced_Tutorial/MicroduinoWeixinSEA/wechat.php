<?php

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

if (strstr($content, "温度")) {
	$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS); 
	mysql_select_db("app_ulink42", $con);//修改数据库名

	$result = mysql_query("SELECT * FROM sensor");
	while($arr = mysql_fetch_array($result)){
	  if ($arr['ID'] == 1) {
	  	$tempr = $arr['data'];
	  }
	}
	mysql_close($con);

    $retMsg = "报告大王："."\n"."主人房间的室温为".$tempr."℃，感谢您对主人的关心";
}else if (strstr($content, "开灯")) {
	$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS); 


	$dati = date("h:i:sa");
	mysql_select_db("app_ulink42", $con);//修改数据库名

	$sql ="UPDATE switch SET timestamp='$dati',state = '1'
	WHERE ID = '1'";//修改开关状态值

	if(!mysql_query($sql,$con)){
	    die('Error: ' . mysql_error());
	}else{
		mysql_close($con);
		$retMsg = "好的主人";
	}
}else if (strstr($content, "关灯")) {
	$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS); 


	$dati = date("h:i:sa");
	mysql_select_db("app_ulink42", $con);//修改数据库名

	$sql ="UPDATE switch SET timestamp='$dati',state = '0'
	WHERE ID = '1'";//修改开关状态值

	if(!mysql_query($sql,$con)){
	    die('Error: ' . mysql_error());
	}else{
		mysql_close($con);
		$retMsg = "好的主人";
	}	
}else{
	$retMsg = "暂时不支持该命令";
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