<?php
include "config.php";
$PID=$_REQUEST['PID'];

//localhost/football/PlayerMatchHistory.php?PID=15
$Get = "SELECT * FROM `matchhistory` WHERE `UserIDs` LIKE '%$PID%' ORDER BY `Time`";

$GetResult=mysqli_query($conn, $Get);

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