import React, { useState, useEffect } from 'react';
import axios from 'axios';

const Dashboard = () => {
  const [attendance, setAttendance] = useState([]);
  const [error, setError] = useState('');

  useEffect(() => {
    const fetchAttendance = async () => {
      try {
        const response = await axios.get('http://your-api-url/attendance');
        setAttendance(response.data);
      } catch (err) {
        setError('Failed to fetch attendance records.');
        console.error(err);
      }
    };
    fetchAttendance();
  }, []);

  return (
    <div>
      <h2>Attendance Records</h2>
      {error && <p style={{ color: 'red' }}>{error}</p>}
      <table>
        <thead>
          <tr>
            <th>Student Name</th>
            <th>Class</th>
            <th>Date</th>
            <th>Status</th>
          </tr>
        </thead>
        <tbody>
          {attendance.map((record, index) => (
            <tr key={index}>
              <td>{record.student_name}</td>
              <td>{record.class_name}</td>
              <td>{record.date}</td>
              <td>{record.status}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
};

export default Dashboard;
