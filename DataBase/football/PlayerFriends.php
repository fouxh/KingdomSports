<?php
include "config.php";
$FNC=$_REQUEST['FNC'];
$PID=$_REQUEST['PID'];
$FriendsID=$_REQUEST['FriendsID'];
$RequestsID=$_REQUEST['RequestsID'];
$Field=$_REQUEST['Field'];
$Value=$_REQUEST['Value'];

//localhost/football/PlayerFriends.php?PID=15&FriendsID=1&RequestsID=1
$Get = "SELECT * FROM playerfriends WHERE PlayerID = '".$PID."'";

$insert = "INSERT INTO `playerfriends`(`PlayerID`, `FriendList`, `Requests`) VALUES ('".$PID."','','')";

$Update = "UPDATE `playerfriends` SET `FriendList`='".$FriendsID."',`Requests`='".$RequestsID."' WHERE PlayerID = '".$PID."'";

//$Update = "UPDATE `playerfriends` SET `".$Field."`=".$Value." WHERE `PlayerID`='".$PID."'";

X:
	 if ($FNC=="Get" )	 {($GetResult=mysqli_query($conn, $Get));} 
else if ($FNC=="Update" ){(mysqli_query($conn, $Update));} 

if (mysqli_num_rows($GetResult) > 0){
	while($row = mysqli_fetch_assoc($GetResult))
	{
		$json_array[]= $row;
	}
} else {
	mysqli_query($conn, $insert);
	goto X;
}

echo json_encode(array('1'=>$json_array));

mysqli_close($conn);
?>