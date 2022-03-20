    <?php

$servername = "localhost";

// REPLACE with your Database name
$dbname = "";
// REPLACE with Database user
$username = "";
// REPLACE with Database user password
$password = "";

// Keep this API Key value to be compatible with the ESP32 code provided in the project page. 
// If you change this value, the ESP32 sketch needs to match
$api_key_value = "tPmAT5Ab3j7F9";

$api_key= $student_id = $location  = "";


if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $api_key = test_input($_POST["api_key"]);
    
    if($api_key == $api_key_value) {
        $student_id = test_input($_POST["student_id"]);
        $location = test_input($_POST["location"]);
        
        //Test_data---------------------------
        /*
        $student_id = '11';
        $location = '1' ;
        $location1='Office';
        
        $time= date('H:i:s',strtotime('7:25:00'));
        $date = date ('Y-m-d',strtotime('2022-01-13'));
        $day_number = date("w", strtotime($date))+1;
        
        //-------------------------------------
        */
         $conn = mysqli_connect($servername, $username, $password, $dbname);
        mysqli_set_charset($conn,'utf8');
        //insert new login data
        $sql = "INSERT INTO LoginData (student_id) VALUES ('" . $student_id . "', '" . $location . "')";
        mysqli_query($conn, $sql);
        echo "New record created successfully".PHP_EOL;
        //
        //select latest reading_time 
        $sql = "SELECT * FROM LoginData  ORDER BY id DESC Limit 1";
        $last_login= mysqli_query($conn,$sql);
        $last_login_data = $last_login->fetch_assoc();
        
        
        //
        
        //split timestamp to date and time
        $time_stamp = $last_login_data['reg_time'];
        $date= date('Y-m-d',strtotime($time_stamp));
        $time= date('H:i:s',strtotime($time_stamp)); 
        echo $date." ".$time.PHP_EOL;
        $day_number = date("w", strtotime($date))+1; // php week 0-6 with 0 for sunday, mysql run 1-7 where 1 for sunday
        $day =  date('l', strtotime($date));
        echo $day." ".$day_number.PHP_EOL;
        
        //find class_code
        $sql= "SELECT * from class_info WHERE  dayofweek= $day_number AND TIME_FORMAT(time_out, '%T') > '$time' ORDER BY TIMEDIFF(time_in, '$time') ASC LIMIT 1";
        //select info of class where time < time_out and closest time_in
        $class_data =  mysqli_query($conn,$sql) or die($conn->error);
        if (mysqli_num_rows($class_data)>0) {
            $class_info = $class_data->fetch_assoc();
            $class_code = $class_info['class_code'];
            $class_time_in = strtotime($class_info['time_in']);
            $time_check_in = strtotime($time);
            echo $class_code." ".$class_time_in.PHP_EOL;
        
            //
            
            //select student_code from student_info
            $sql ="SELECT * FROM student_info WHERE esp_data_num = $student_id ";
            $student_info =  mysqli_query($conn,$sql) or die($conn->error);
            if (mysqli_num_rows($student_info)>0) {
                $student_info_detail = $student_info ->fetch_assoc();
                $student_code = $student_info_detail['student_code'];
                echo $student_code.PHP_EOL;
                
                //select student_code,class_code from login_checkin, check for latest checkin, if not alr attend+1 for time < timein+15, late +1 for time > timein+15
                $sql= "SELECT * FROM login_check_in WHERE student_code= '$student_code' AND class_code = '$class_code'";
                $check_in_info = mysqli_query($conn,$sql) or die($conn->error);
                if (mysqli_num_rows($check_in_info)>0) {
                    $check_in_detail = $check_in_info -> fetch_assoc();
                    $last_check_in = $check_in_detail['last_check_in'];
                    echo ($last_check_in!=$date)?"new ":"alr ";
                    //new check in
                    if ($last_check_in!=$date){
                        $sql_attend="UPDATE login_check_in SET num_attend= num_attend+1, last_check_in = '$date' WHERE student_code= '$student_code' AND class_code = '$class_code'";
                        $sql_late = "UPDATE login_check_in SET num_late= num_late+1, last_check_in = '$date' WHERE student_code= '$student_code' AND class_code = '$class_code'";
                        $sql = (round(($time_check_in-$class_time_in)/60) < 15)? $sql_attend:$sql_late;
                        $check_in_result = mysqli_query($conn,$sql) or die($conn->error);
                        echo "New Check in created".PHP_EOL;
                    }
                }
                else
                echo "Student doesn't belong in this class".PHP_EOL;
            }
            else echo "No student data".PHP_EOL;
        }
        else echo "No Class this time".PHP_EOL;
        
        
        //--------------------------------------
        $conn->close();


    }
    else {
        echo "Wrong API Key provided.";
    }

}
else {
    echo "No data posted with HTTP POST.";
}


function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
