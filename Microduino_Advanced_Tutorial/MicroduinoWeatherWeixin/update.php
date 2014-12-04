<?php


$mydatabase="app_leirobot";

if ($_GET['json'] && ($_GET['token'] == "doubleq")) {//可以改token,这相当于密码，在Arduino端改成相应的值即可
	$con = mysql_connect(SAE_MYSQL_HOST_M.':'.SAE_MYSQL_PORT,SAE_MYSQL_USER,SAE_MYSQL_PASS);


	//传过来的json格式：{"temperature":25,"humidity":40,"illumination":89,"pressure":101}

	$json = $_GET['json'];
	mysql_select_db($mydatabase, $con);//要改成相应的数据库名
	$result = mysql_query("SELECT * FROM switch");
	while($arr = mysql_fetch_array($result)){//找到需要的数据的记录，并读出状态值
		if ($arr['ID'] == 1) {
			$state = $arr['state'];
		}
	}
	$dati = date("h:i:sa");//获取时间

	$data=json_decode($json,true);

	$temperature=$data["temperature"];
	$humidity=$data["humidity"];
	$illumination=$data["illumination"];
	$pressure=$data["pressure"];


	$sql ="UPDATE sensor SET timestamp='$dati',
	data = (CASE ID WHEN 1 THEN '$temperature'
                    WHEN 2 THEN '$humidity'
                    WHEN 3 THEN '$illumination'
                    WHEN 4 THEN '$pressure'
             END)
	WHERE ID IN(1,2,3,4)";//更新相应的传感器的值

	if(!mysql_query($sql,$con)){
		die('Error: ' . mysql_error());//如果出错，显示错误
	}
	mysql_close($con);
	echo "{".$state."}";//返回状态值，加“{”是为了帮助Arduino确定数据的位置
}else{
	echo "Permission Denied";//请求中没有type或data或token或token错误时，显示Permission Denied
}
?>