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


CREATE TABLE Course_registration(
    course_code    TEXT,
    student_id     INTEGER,
    PRIMARY KEY (course_code, student_id),
    FOREIGN KEY (course_code) REFERENCES Course(code),
    FOREIGN KEY (student_id) REFERENCES Student(id)
);

CREATE TABLE Attendance_Log (
    student_id     INTEGER,
    course_code    TEXT,
    recorded_on    TEXT,
    recorded_at    TEXT,
    PRIMARY KEY (student_id, course_code, recorded_on),
    FOREIGN KEY (student_id)  REFERENCES Student(id),
    FOREIGN KEY (course_code) REFERENCES Course(code)
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

INSERT INTO Class_Schedule (class_id, student_id) VALUES
('SOEN 422', 101),
('SOEN 422', 103),
('COEN 320', 102),
('COEN 320', 103);

