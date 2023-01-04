<?php
include "config.php";
$playerId=$_REQUEST['id'];
$Score=$_REQUEST['Score'];
if($playerId && $Score)
{
	

//insert  code
//$sql="INSERT INTO `leaderboard`( `PlayerId`, `Score`) VALUES ('playerid1',50)";
//update code

$sql="UPDATE `leaderboard` SET `Score`=`Score`+$Score WHERE `PlayerId`='".$playerId."'";
if (mysqli_query($conn, $sql) === TRUE) {
  echo "New record created successfully";
} else {
  echo "Error: " ;
}

mysqli_close($con);

}
else
{
	echo "inputs needed";
}
?>