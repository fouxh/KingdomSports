<?php
include "config.php";
$PlayerID=$_REQUEST['PlayerID'];

//localhost/football/PlayerProfile.php?PlayerID=	
$Get = "SELECT * FROM player_info WHERE PlayerID = '".$PlayerID."'";

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