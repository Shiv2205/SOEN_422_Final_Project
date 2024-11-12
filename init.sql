CREATE TABLE RFID_Signature ( 
    RFID      VARCHAR(255)  PRIMARY KEY,
    type      VARCHAR(255) NOT NULL,
    CONSTRAINT chk_type CHECK (type IN ('Professor', 'Student'))
);

CREATE TABLE Professor ( 
    id        INT           PRIMARY KEY,
    name      VARCHAR(255),
    email     VARCHAR(255),
    RFID_Sig  VARCHAR(255),
    FOREIGN KEY (RFID_Sig) REFERENCES RFID_Signature(RFID)
);

CREATE TABLE Student ( 
    id        INT           PRIMARY KEY,
    name      VARCHAR(255),
    email     VARCHAR(255),
    RFID_Sig  VARCHAR(255),
    FOREIGN KEY (RFID_Sig) REFERENCES RFID_Signature(RFID)
);

CREATE TABLE Room ( 
    room_number VARCHAR(255) PRIMARY KEY
);

CREATE TABLE Course ( 
    code         VARCHAR(255),
    section      VARCHAR(255),
    professor_id INT,
    PRIMARY KEY (code, section),
    FOREIGN KEY (professor_id) REFERENCES Professor(id)
);

CREATE TABLE Class ( 
    id             VARCHAR(255)  PRIMARY KEY,
    course_code    VARCHAR(255),
    course_section VARCHAR(255),
    room_number    VARCHAR(255),
    start_time     TIME,
    end_time       TIME,
    FOREIGN KEY (course_code, course_section) REFERENCES Course(code, section),
    FOREIGN KEY (room_number)                 REFERENCES Room(room_number)
);

CREATE TABLE Class_Schedule (
    class_id    VARCHAR(255),
    student_id  VARCHAR(255),
    PRIMARY KEY (class_id, student_id)
);

CREATE TABLE Room_Log (
    room_number VARCHAR(255),
    RFID_Sig    VARCHAR(255),
    recorded_on DATE,
    recorded_at DATETIME,
    PRIMARY KEY (room_number, RFID_Sig),
    FOREIGN KEY (room_number)   REFERENCES Room(room_number),
    FOREIGN KEY (RFID_Sig)      REFERENCES RFID_Signature(RFID)
);

INSERT INTO RFID_Signature (RFID, type) VALUES
('RFID123', 'Professor'),
('RFID124', 'Student'),
('RFID125', 'Student'),
('RFID126', 'Professor');

INSERT INTO Professor (id, name, email, RFID_Sig) VALUES
(1, 'Dr. John Smith', 'john.smith@university.edu', 'RFID123'),
(2, 'Dr. Emily Carter', 'emily.carter@university.edu', 'RFID126');

INSERT INTO Student (id, name, email, RFID_Sig) VALUES
(101, 'Alice Johnson', 'alice.johnson@university.edu', 'RFID124'),
(102, 'Bob Williams', 'bob.williams@university.edu', 'RFID125');

INSERT INTO Room (room_number) VALUES
('A101'),
('B202'),
('C303');

INSERT INTO Course (code, section, professor_id) VALUES
('CS101', 'A', 1),
('CS102', 'B', 2);

INSERT INTO Class (id, course_code, course_section, room_number, start_time, end_time) VALUES
('CL101', 'CS101', 'A', 'A101', '09:00:00', '10:30:00'),
('CL102', 'CS102', 'B', 'B202', '11:00:00', '12:30:00');

INSERT INTO Class_Schedule (class_id, student_id) VALUES
('CL101', 101),
('CL101', 102),
('CL102', 101);

INSERT INTO Room_Log (room_number, RFID_Sig, recorded_on, recorded_at) VALUES
('A101', 'RFID123', '2024-11-11', '2024-11-11 09:05:00'),
('A101', 'RFID124', '2024-11-11', '2024-11-11 09:10:00'),
('B202', 'RFID126', '2024-11-11', '2024-11-11 11:15:00'),
('B202', 'RFID125', '2024-11-11', '2024-11-11 11:20:00');
