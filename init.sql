CREATE TABLE RFID_Signature ( 
    RFID      TEXT PRIMARY KEY,
    type      TEXT NOT NULL CHECK (type IN ('Professor', 'Student'))
);

CREATE TABLE Professor ( 
    id        INTEGER PRIMARY KEY,
    name      TEXT,
    email     TEXT,
    RFID_Sig  TEXT,
    FOREIGN KEY (RFID_Sig) REFERENCES RFID_Signature(RFID)
);

CREATE TABLE Student ( 
    id        INTEGER PRIMARY KEY,
    name      TEXT,
    email     TEXT,
    RFID_Sig  TEXT,
    FOREIGN KEY (RFID_Sig) REFERENCES RFID_Signature(RFID)
);

CREATE TABLE Course ( 
    code         TEXT,
    section      TEXT,
    professor_id INTEGER,
    PRIMARY KEY (code, section),
    FOREIGN KEY (professor_id) REFERENCES Professor(id)
);

CREATE TABLE Class ( 
    id             TEXT PRIMARY KEY,
    course_code    TEXT,
    course_section TEXT,
    start_time     TEXT,
    end_time       TEXT,
    FOREIGN KEY (course_code, course_section) REFERENCES Course(code, section)
);

CREATE TABLE Class_Schedule (
    class_id    TEXT,
    student_id  INTEGER,
    PRIMARY KEY (class_id, student_id),
    FOREIGN KEY (class_id) REFERENCES Class(id),
    FOREIGN KEY (student_id) REFERENCES Student(id)
);

CREATE TABLE Attendance_Log (
    RFID_Sig    TEXT,
    recorded_on TEXT,
    recorded_at TEXT,
    PRIMARY KEY (RFID_Sig, recorded_on, recorded_at),
    FOREIGN KEY (RFID_Sig) REFERENCES RFID_Signature(RFID)
);

-- Insert data
INSERT INTO RFID_Signature (RFID, type) VALUES
('E1 11 30 00', 'Professor'),
('D3 DE B6 14', 'Student'),
('6B CB A2 59', 'Student'),
('59 48 51 C1', 'Student'),
('59 48 51 C2', 'Professor');

INSERT INTO Professor (id, name, email, RFID_Sig) VALUES
(1, 'Dr. Sandra Céspedes', 'sandra.cespedes@university.edu', 'E1 11 30 00'),
(2, 'Dr. Aiman Hanna', 'aiman.hanna@university.edu', '59 48 51 C2');

INSERT INTO Student (id, name, email, RFID_Sig) VALUES
(101, 'Alice Johnson', 'alice.johnson@university.edu', 'D3 DE B6 14'),
(102, 'Bob Williams', 'bob.williams@university.edu', '6B CB A2 59'),
(103, 'Shivam Veerabudren', 'shiv.veera@university.edu', '59 48 51 C1');

INSERT INTO Course (code, section, professor_id) VALUES
('SOEN 422', 'MM', 1),
('COEN 320', 'S', 2);

INSERT INTO Class (id, course_code, course_section, start_time, end_time) VALUES
('CL101', 'SOEN 422', 'MM', '09:00:00', '10:30:00'),
('CL102', 'COEN 320', 'S', '11:00:00', '12:30:00');

INSERT INTO Class_Schedule (class_id, student_id) VALUES
('CL101', 101),
('CL101', 103),
('CL102', 102),
('CL102', 103);

INSERT INTO Attendance_Log (RFID_Sig, recorded_on, recorded_at) VALUES
('RFID123', '2024-11-11', '2024-11-11 09:05:00'),
('RFID124', '2024-11-11', '2024-11-11 09:10:00'),
('RFID126', '2024-11-11', '2024-11-11 11:15:00'),
('RFID125', '2024-11-11', '2024-11-11 11:20:00');
