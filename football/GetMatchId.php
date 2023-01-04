<?php
//include "config.php";
$id_team=$_REQUEST['id_team'];
$return_arr = array();
 $sqlteam="SELECT `id_mt`, `red_team`, `blue_team`, `Status`, `Created` FROM `match_info` WHERE  `red_team`='".$id_team."' OR `blue_team`='".$id_team."' LIMIT 1";
// echo  $sqlteam;
	$result=mysqli_query($conn, $sqlteam);

 if (mysqli_num_rows($result) > 0) 
{
 while($row = mysqli_fetch_assoc($result)) 
 {
	  $return_arr['id']=$row["id_mt"];
	  $return_arr['red_team']=$row["red_team"];
	  $return_arr['blue_team']=$row["blue_team"];
	   
  }  

}
else
{
$return_arr['id']="Error";
}
echo json_encode($return_arr);
mysqli_close($con);

?>
