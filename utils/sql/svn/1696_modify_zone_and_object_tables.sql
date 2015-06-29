ALTER TABLE object MODIFY objectname varchar(32);
ALTER TABLE zone MODIFY short_name varchar(32);
ALTER TABLE spawn2 MODIFY zone varchar(32);
ALTER TABLE doors MODIFY zone varchar(32);
ALTER TABLE zone_points MODIFY zone varchar(32);
ALTER TABLE zone_state_dump MODIFY zonename varchar(32);
ALTER TABLE launcher_zones MODIFY zone varchar(32);
ALTER TABLE fear_hints MODIFY zone varchar(32);
ALTER TABLE spawn_conditions MODIFY zone varchar(32);
ALTER TABLE spawn_events MODIFY zone varchar(32);
UPDATE zone SET short_name = "oceangreenvillage" WHERE short_name = "oceangreenvillag";
UPDATE spawn2 SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE doors SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE zone_points SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE zone_state_dump SET zonename = "oceangreenvillage" WHERE zonename = "oceangreenvillag";
UPDATE launcher_zones SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE fear_hints SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE spawn_conditions SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";
UPDATE spawn_events SET zone = "oceangreenvillage" WHERE zone = "oceangreenvillag";