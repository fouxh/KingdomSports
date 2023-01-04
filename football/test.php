<?php
include "config.php";
$playerId=$_REQUEST['id'];
$Score=$_REQUEST['Score'];
if($playerId && $Score)
{
	

//insert  code
//$sql="INSERT INTO `leaderboard`( `PlayerId`, `Score`) VALUES ('playerid1',50)";
//update code

$sql = "SELECT `Id`, `PlayerId`, `Score` FROM `leaderboard` WHERE `PlayerId`='".$playerId."'";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
  // output data of each row
$sql="UPDATE `leaderboard` SET `Score`=`Score`+$Score WHERE `PlayerId`='".$playerId."'";
} else {
 $sql="INSERT INTO `leaderboard`( `PlayerId`, `Score`) VALUES ('".$playerId."',$Score)";
}
mysqli_query($conn, $sql);
mysqli_close($conn);


}
else
{
	echo "inputs needed";
}
?>

