import cv2
import mediapipe as mp
from PyQt5 import QtWidgets, QtCore


class PoseTrackingThread(QtCore.QThread):
    progress_updated = QtCore.pyqtSignal(int)
    frames_updated = QtCore.pyqtSignal(list)

    def __init__(self, filename):
        super().__init__()
        self.filename = filename

    def run(self):
        mp_drawing = mp.solutions.drawing_utils
        mp_pose = mp.solutions.pose

        cap = cv2.VideoCapture(self.filename)

        frames = []

        total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))

        for current_frame_pos in range(total_frames):
            ret, frame = cap.read()

            if not ret:
                break

            with mp_pose.Pose(static_image_mode=False, min_detection_confidence=0.5, min_tracking_confidence=0.5) as pose:
                Img = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
                results = pose.process(Img)

                if results.pose_landmarks:
                    t_coords = []

                    temp_results = results.pose_landmarks
                    for landmark in results.pose_landmarks.landmark:
                        t_coords.append({"x": landmark.x, "y": landmark.y, "z": landmark.z})
                elif temp_results:
                    for landmark in temp_results.landmark:
                        t_coords.append({"x": landmark.x, "y": landmark.y, "z": landmark.z})
                else:
                    pass# Codes when the first frames are not tracked by meadiapipe

                # Emit the progress signal to update the progress bar
                self.progress_updated.emit(current_frame_pos + 1)
                frames.append(t_coords)
        self.frames_updated.emit(frames)
        cap.release()



class Video:
    def __init__(self, type : str, filename : str, progressbar):
        self.frames = []
        self.type = type
        self.filename = filename
        self.currentframe_pos = 0
        self.bar = progressbar

        if self.filename:
            self.track_poses()

    def add_frames(self, frame):
        self.frames.append(frame)

    def draw_tracked_points(self, img, frame_pos):
        coords = self.frames[frame_pos]
                        
        for coord in coords:
            x = coord["x"]
            y = coord["y"]

            x_px, y_px = int(x * img.shape[1]), int(y * img.shape[0])
            cv2.circle(img, (x_px, y_px), 3, (0, 255, 0), -1)


# Tracking poses on Import
    def track_poses(self):
        cap = cv2.VideoCapture(self.filename)
        self.frame_rate = int(cap.get(cv2.CAP_PROP_FPS))
        self.total_frames = int(cap.get(cv2.CAP_PROP_FRAME_COUNT))
        cap.release()

        self.pose_thread = PoseTrackingThread(self.filename)
        self.pose_thread.progress_updated.connect(self.update_progress)
        self.pose_thread.frames_updated.connect(self.update_frames)
        self.pose_thread.start()

    def update_progress(self, current_frame_pos):
        self.currentframe_pos = current_frame_pos
        self.bar.setValue(self.currentframe_pos)
        if self.currentframe_pos == self.total_frames:
            self.bar.close()

    def update_frames(self, frames):
        self.frames = frames

    def getThumbnail(self):
        self.vidcap = cv2.VideoCapture(self.filename)
        self.vidcap.set(cv2.CAP_PROP_POS_FRAMES, 5)
        ret, frame = self.vidcap.read()
        self.vidcap.release()
        
        if ret:
            image = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            return image