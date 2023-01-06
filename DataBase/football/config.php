<?php 
header('Content-Type: application/json');
error_reporting(0);
date_default_timezone_set('Africa/Cairo');
$servername = "localhost";
$username = "root";
$password = "";
$dbname = "football";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
// Check connection
if (!$conn) {
  die("Connection failed: " . mysqli_connect_error());
}
function CheckValueDb($tbl, $condition_fx)
 {
	 $servername = "localhost";
$username = "root";
$password = "";
$dbname = "football";

// Create connection
$conn = mysqli_connect($servername, $username, $password, $dbname);
$sglfx="SELECT * FROM ".$tbl." WHERE ".$condition_fx." LIMIT 1" ;
//echo $sglfx;
$res= mysqli_query($conn,$sglfx);

 if (mysqli_num_rows($res) > 0)
 {
	 return true ;
 } 
else
{
 return false ;	
} 
 
}
?>