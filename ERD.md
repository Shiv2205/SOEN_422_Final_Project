```mermaid
erDiagram

    RFID_Signature ||--|{ Room_Log : "is logged in"    
    RFID_Signature {
      string RFID PK
      enum type "Professor, Student"
    }

    Professor ||--|| RFID_Signature : has
    Professor {
      int id PK
      string name
      string email
      string RFID_Sig FK
    }

    Student ||--|| RFID_Signature : has
    Student ||--|{ Class_Schedule : has
    Student {
      int id PK
      string name
      string email
      string RFID_Sig FK
    }

    Room ||--|{ Class : hosts
    Room ||--|{ Room_Log : "is part of"
    Room {
      string room_number PK
    }

    Course ||--|{ Class : "is taught in"
    Course {
      string code PK "SOEN 422"
      string section PK
      int professor_id FK
    }

    Class ||--|{ Class_Schedule : "is in"
    Class {
      string id PK
      string course_code FK
      string course_section FK
      string room_number FK
      time start
      time end
    }

    Class_Schedule {
      string class_id PK,FK
      int student_id PK,FK
    }

    Room_Log {
      string room_number PK,FK
      string RFID_Sig PK,FK
      date Date "Date this record was created"
      timestamp time "Time the card was scanned"
    }

```