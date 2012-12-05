<?php
// --------------------------------------------------------------------------
// File name   : RS232_Server.php
//
//Procedure Description: 
//RS232_Client the procedures and the demonstration of a complete system in PHP to display 
//the RS232 serial communications applications. 
//Process to achieve a foundation of integrity, however the RS232 communication protocol 
//(HQB232), communication protocol format is as follows: 
//Agreement: 
//C-> S 01 //request for communication 
//S-> C 02 //response communications 
//C-> S LEN DATA //LEN said data (DATA) that the actual data length of DATA 
//C-> S 03 //end communication 
//Note: S, said that under the PC C-bit machine 
//HQB232 said HonestQiao the basis of presentation (Base) RS232 communication protocol, 
//including the agreement 
//Request and response, data frame structure. 
//Demonstration of the process for request and response communications, document
//delivery (data frame), the end of communication 
//Data frame structure of the current data for the current length of the HEX data 
//string (2 complement)
// --------------------------------------------------------------------------
set_time_limit(0);
exec('mode COM3: baud=38400 data=8 stop=1 parity=n xon=on');
 
$fd = dio_open('COM3:', O_RDWR);
if(!$fd)
{
        die("Error when open COM3");
}
 
$ff = dio_stat($fd);print_r($ff);
echo "HQB232 SERVER is listenning on COM3\n";
/// read
$len = 2;
$t=0;while (($t++)<1000) 
{
        $data = dio_read($fd, $len);
        if ($data) {
                if($data==chr(0).chr(1)){
                        echo "S_RECV:01\n";
                        echo "S_SEND:02\n";
                        dio_write($fd,chr(0).chr(2));
                        break;
                }
        }
}
 
$handle = fopen("E:\\RS232\\TEST\\S_WAPMMonitor.exe", 'w');
/// read
$len = 2;
$t=0;while ($handle && ($t++)<1000) 
{
        $len = 2;
        $data = dio_read($fd, $len);
        if($data==chr(0).chr(3)){
                echo "S_RECV:03\n";
                break;
        }
        elseif ($data) {
                $len = hexdec($data);
                if($len>0)
                {
                $data = dio_read($fd, $len);
                if($data){
                        fwrite($handle, str2hex($data));
                        echo "S_RECV:($len) ".microtime()."\n";
                }
                }
        }
}
fclose($handle);
dio_close($fd);
 
function str2hex($string)
{
        $str = '';
        for($i=0; $i<strlen($string); $i+=2){
                $str .= chr(hexdec(substr($string,$i,2)));
        }
        return $str;
}
?>