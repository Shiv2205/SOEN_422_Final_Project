
```sql

Table RFID_Index{
  RFID varachar [pk]
  type enum("Teacher","Student")    
}

Table Rooms {
  room_number varchar [pk]
}

Table Professors{
  id integer [pk]
  name varchar
  email varchar
  RFID_Sig varchar [ref:- RFID_Index.RFID]
}

Table Students{
  id ineteger [pk]
  name varchar
  email varchar
  RFID_Sig varchar [ref:- RFID_Index.RFID]
}

Table Courses{
  code varchar
  id integer
  section varchar
  professor_id ineteger [ref:> Professors.id]
  room_number varchar [ref:> Rooms.room_number]

  
}

Table Course_Taken{
  course_code varchar
  course_id integer
  course_section varchar
  student_id integer [ref:> Students.id]

  indexes {
    ( course_code,
      course_id,
      course_section,
      student_id) [pk]
  }
}

Ref: Course_Taken.(course_code,course_id, course_section) > Courses.(code,id,section)

```