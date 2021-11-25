<?php
include "config.php";
$ClanID=$_REQUEST['ClanID'];

$Get = "SELECT clan.ClanID AS ClanID, players.PlayerID AS PlayerID FROM clan,players WHERE players.ClanID & clan.ClanID = '".$ClanID."'";

$GetResult=mysqli_query($conn, $Get);

if (mysqli_num_rows($GetResult) > 0) 
{
	while($row = mysqli_fetch_assoc($GetResult)) 
	{
		$ClanID = $row['ClanID'];
		$PlayerID = $row['PlayerID'];
		$json_array[]= $row;
	}
}
echo json_encode(array('1'=>$json_array));

mysqli_close($conn);
?>