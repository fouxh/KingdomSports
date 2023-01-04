<?php
include "config.php";
$ClanID=$_REQUEST['ClanID'];
$PlayerID=$_REQUEST['Pl'];
$json_array;
//localhost/football/ClanMembers.php?ClanID=6
//problem->empty string in PL_id
$sql = "SELECT `Id`, `Pl_id`, `ClanId` FROM `playerclan` WHERE `Pl_id`='".$PlayerID."' AND `ClanId`=$ClanID ";
$result = mysqli_query($conn, $sql);

if (mysqli_num_rows($result) > 0) {
  // output data of each row
  while($row = mysqli_fetch_assoc($result))
	{
  $json_array[]=$row;
	}

} else {

 $sql="INSERT INTO `playerclan`(`Pl_id`, `ClanId`) VALUES ('".$PlayerID."',$ClanID)";
 $json_array["id"]="Clan Created Successfully";
}
mysqli_query($conn, $sql);
mysqli_close($conn);
/*
INSERT INTO `playerclan`(`Id`, `Pl_id`, `ClanId`) VALUES ('[value-1]','[value-2]','[value-3]')
$GetResult=mysqli_query($conn, $Get);

if (mysqli_num_rows($GetResult) > 0)
{
	while($row = mysqli_fetch_assoc($GetResult))
	{
		$ClanID = $row['ClanID'];
		$PlayerID = $row['PlayerID'];
		$json_array[]= $row;
	}
}*/
echo json_encode($json_array);

mysqli_close($conn);
?>
