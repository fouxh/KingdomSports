<?php
include "config.php";
$PlayerID=$_REQUEST['PlayerID'];
$ItemID=$_REQUEST['ItemID'];

//localhost/football/Unlocked_Items_Insert.php?PlayerID=15&ItemID=2	
$Get = "INSERT INTO `unlocked_items`(`itemID`, `playerID`) VALUES('".$ItemID."','".$PlayerID."')";

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