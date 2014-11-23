<?php

	// Load JSON state
    $string = file_get_contents("robot_state.json");
    $json_a= json_decode($string,true);

    // Handle GET request
    $json_a['speed_left'] = $_GET["speedLeft"];
    $json_a['speed_right'] = $_GET["speedRight"];
    $json_a['direction_left'] = $_GET["directionLeft"];
    $json_a['direction_right'] = $_GET["directionRight"];

    // Save JSON file
    $fp = fopen('robot_state.json', 'w');
    fwrite($fp, json_encode($json_a));
    fclose($fp);

    // Create a TCP/IP socket & connect to the server
    $socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
    $result = socket_connect($socket, "192.168.1.109", "8888");

    // Request
    //$in = "HEAD / HTTP/1.1\r\n";
    //$in .= "Content-Type: text/html\r\n";
    $in = $json_a['speed_left'] . "," . 
    $json_a['speed_right'] . "," .
    $json_a['direction_left'] . "," .
    $json_a['direction_right'] . ",\r\n\r\n";



    $out = '';

    // Send request
    socket_write($socket, $in, strlen($in));

    // Read answer
    while ($out = socket_read($socket, 4096)) {
        echo $out;
    }

    // Close socket
    socket_close($socket);

?>