<?php
include "config.php";
$PID=$_REQUEST['PID'];
$FNC=$_REQUEST['FNC'];
$Value=$_REQUEST['Value'];

//localhost/football/PlayerPass.php?FNC=1&PID=15
$Get = "SELECT * FROM `playerpass` WHERE `PlayerID`='$PID'";
//localhost/football/PlayerPass.php?FNC=2&PID=15&Value=
$Update = "UPDATE `playerpass` SET `CollectStatus`='$Value' WHERE `PlayerID`='$PID'";



	 if ($FNC==1)	{($GetResult=mysqli_query($conn, $Get));}
else if ($FNC==2)	{($GetResult=mysqli_query($conn, $Update));}

if (mysqli_num_rows($GetResult) > 0) 
{
	while($row = mysqli_fetch_assoc($GetResult)) 
	{
		$json_array[]= $row;
	}
}
echo json_encode(array('1'=>$json_array));

mysqli_close($conn);
?>