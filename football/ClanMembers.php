<?php
include "config.php";
$ClanID=$_REQUEST['ClanID'];

//localhost/football/ClanMembers.php?ClanID=6
$Get = $sqlClans = "SELECT `ID`,`Flag`,`Trophies`,`Name` FROM `players` WHERE `ClanID`='$ClanID'";

$result=mysqli_query($conn, $Get);

if (mysqli_num_rows($result) > 0)
  {
  $result_array["data"]=mysqli_fetch_all($result, MYSQLI_ASSOC);
  echo json_encode($result_array);
  }
else
  {
  mysqli_close($conn);
  }
mysqli_close($conn);
?>
