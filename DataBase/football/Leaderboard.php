<?php
include "config.php";
$FNC=$_REQUEST['FNC'];
$Type=$_REQUEST['Type'];
$PID=$_REQUEST['PID'];

//localhost/football/Leaderboard.php?Type=Trophy
$SL = "SELECT player_info.Goals AS Goals, players.Name AS Name, players.Flag AS Flag, players.Trophies AS Trophies FROM player_info,players WHERE player_info.PlayerID = players.PlayerID ORDER BY `$Type` DESC LIMIT 10";

$ST = "SELECT  x.Name, x.Flag, x.Trophies,y.Goals,(SELECT COUNT(*) FROM players t WHERE t.Trophies >= x.Trophies) AS Rank FROM players x, player_info y WHERE x.PlayerID = y.PlayerID AND x.PlayerID = '$PID'";

$SG = "SELECT x.Name, x.Flag, x.Trophies, y.Goals,(SELECT COUNT(*) FROM player_info t WHERE t.Goals >= y.Goals) AS Rank FROM players x, player_info y WHERE x.PlayerID = y.PlayerID AND x.PlayerID = '$PID'";

	 if ($FNC==1)	{($GetResult=mysqli_query($conn, $SL));}
else if ($FNC==2)	{($GetResult=mysqli_query($conn, $ST));}
else if ($FNC==3)	{($GetResult=mysqli_query($conn, $SG));}

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