<?php
include "config.php";
$PID=$_REQUEST['PID'];
$FNC=$_REQUEST['FNC'];
$Name=$_REQUEST['Name'];
$Status=$_REQUEST['Status'];
$Flag=$_REQUEST['Flag'];
$Gem=$_REQUEST['Gem'];
$Gold=$_REQUEST['Gold'];
$PlayerChar=$_REQUEST['PlayerChar'];
$Stars=$_REQUEST['Stars'];
$Trophies=$_REQUEST['Trophies'];
$Energy=$_REQUEST['Energy'];
$Pass=$_REQUEST['Pass'];
$Field=$_REQUEST['Field'];
$Value=$_REQUEST['Value'];
$Tbl=$_REQUEST['Tbl'];


$sql = "INSERT INTO `players`(`Name`, `Status`, `Flag`, `Gem`, `Gold`, `PlayerChar`, `Stars`, `Trophies`, `Energy`, `Pass`) 
VALUES ('".$Name."','".$Status."','".$Flag."','".$Gem."','".$Gold."','".$PlayerChar."','".$Stars."','".$Trophies."','".$Energy."','".$Pass."')";

//localhost/football/PlayerMod.php?FNC=NewUser&Name=test
$NewUser = "INSERT INTO `players`(`Name`, `Flag`) VALUES('".$Name."','".$Flag."')";

//localhost/football/PlayerMod.php?FNC=Update&Field=Gold&Value=300&PID=10
$Update = "UPDATE `players` SET `".$Field."`=".$Value." WHERE `PlayerID`='".$PID."'";

//localhost/football/PlayerMod.php?FNC=Get&Field=Gold&PID=10
$Get = "SELECT `".$Field."` FROM `players` WHERE `PlayerID`='".$PID."'";

//localhost/football/PlayerMod.php?FNC=GetAll&PID=10
$GetAll = "SELECT * FROM `players` WHERE `PlayerID`='".$PID."'";

//localhost/football/PlayerMod.php?FNC=Delete&PID=10
$Delete = "DELETE FROM `players` WHERE `PlayerID`='".$PID."'";

	 if ($FNC==1)		 {(mysqli_query($conn, $sql ));} 
else if ($FNC=="NewUser"){(mysqli_query($conn, $NewUser));} 
else if ($FNC=="Update" ){($GetResult=mysqli_query($conn, $Update));} 
else if ($FNC=="Get")	 {($GetResult=mysqli_query($conn, $Get));}
else if ($FNC=="GetAll") {($GetResult=mysqli_query($conn, $GetAll));}
else if ($FNC=="Delete") {(mysqli_query($conn, $Delete));}

if (mysqli_num_rows($GetResult) > 0) 
{
	while($row = mysqli_fetch_assoc($GetResult)) 
	{
		$json_array[] = $row;
	}
}

echo json_encode(array('1'=>$json_array));

mysqli_close($conn);
?>