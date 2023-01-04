<?php
include "config.php";
$isjoin=$_REQUEST['isjoin'];
$clan_name=$_REQUEST['clan_name'];
$PlayerID=$_REQUEST['PlayerID'];
$ClanID;
$json_array;
//localhost/football/JoinClan.php?isjoin=0&clan_name=TestClan&PlayerID=23

$sql = "SELECT `clan_name` FROM `players` WHERE `ID`='$PlayerID.'";
$result = mysqli_query($conn, $sql);
if ($isjoin==0)
{
  if (mysqli_num_rows($result) > 0)
  {
    //Update Number of members in clan_name
    $sqlclan= "UPDATE `clans` SET `num_members`=`num_members`+'1' WHERE `clan_name`='".$clan_name."'";
    mysqli_query($conn, $sqlclan);


    $sqlclanid = "SELECT `clan_id` FROM `clans` WHERE `clan_name`='".$clan_name."'";
    $resultid= mysqli_query($conn, $sqlclanid);

    //Json for Unreal
    $json_array["ClanID"]=mysqli_fetch_all($resultid, MYSQLI_ASSOC);
    $json_array["message"]="Player Joined Clan Successfully";
    echo json_encode($json_array);

    //Update Player's ClanID, ClanName
    $localid= $json_array["ClanID"][0]["clan_id"];
    $sqladd= "UPDATE `players` SET `ClanID`='$localid', `clan_name`='".$clan_name."' WHERE `ID`='$PlayerID'";
    mysqli_query($conn, $sqladd);

  }
  else
  {
    //Couldn't find player
    $json_array["message"]="Can't Find Player";
    echo json_encode($json_array);
  }
}
else
  {
    //Update Number of members in clan_name
    $sqlclan= "UPDATE `clans` SET `num_members`=`num_members`-'1' WHERE `clan_name`='".$clan_name."'";
    mysqli_query($conn, $sqlclan);

    //Update Player's ClanID, ClanName
    $sqladd= "UPDATE `players` SET `ClanID`=NULL, `clan_name`=NULL WHERE `ID`='$PlayerID'";
    mysqli_query($conn, $sqladd);

    $json_array["message"]="Player Left Clan Successfully";
    echo json_encode($json_array);
  }

mysqli_close($conn);

?>
