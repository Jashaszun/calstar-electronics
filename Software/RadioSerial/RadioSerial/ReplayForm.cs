using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.IO;

namespace RadioSerial
{
    public partial class ReplayForm : Form
    {
        System.Timers.Timer replayTimer;
        double speed = 1.0;
        long timeOfLastTick = -1;
        public ReplayForm(string logFile)
        {
            InitializeComponent();

            this.Text = "Replay Log: " + Path.GetFileName(logFile);

            this.tryAction(() =>
            {
                string[] lines = File.ReadAllLines(logFile);
                Tuple<long, string>[] messages = readLog(lines);

                long messagesStart = messages[0].Item1;
                long messagesEnd = messages[messages.Length - 1].Item1;

                trackBar.Minimum = 0;
                trackBar.Maximum = messages.Length - 1;
                trackBar.Value = 0;
                maxValueLabel.Text = ((double)((messagesEnd - messagesStart) / 10000)) / 1000 + "s";

                replayTimer = new System.Timers.Timer(20);
                int lastMessageAdded = -1;
                long startTime = -1;
                long totalReplayTime = 0;
                object lockObj = 1;
                replayTimer.Elapsed += (IDidNawtHitHer, OhHaiMark) =>
                {
                    this.tryAction(() =>
                    {
                        if (timeOfLastTick == -1)
                        {
                            startTime = timeOfLastTick = DateTime.Now.ToFileTimeUtc();
                            totalReplayTime = 0;
                        }
                        else
                        {
                            long currentTime = DateTime.Now.ToFileTimeUtc();
                            totalReplayTime += (long)(speed * (currentTime - timeOfLastTick));
                            timeOfLastTick = currentTime;
                        }

                        lock (lockObj)
                        {
                            int nextMessage = lastMessageAdded + 1;
                            while (nextMessage < messages.Length &&
                                messages[nextMessage].Item1 - messagesStart <= totalReplayTime)
                            {
                                long time = messages[nextMessage].Item1;
                                string line = messages[nextMessage].Item2;
                                MainForm.addReplayLine(startTime + (time - messagesStart), line);
                                lastMessageAdded++;
                                nextMessage++;
                            }
                            if (lastMessageAdded == messages.Length - 1)
                            {
                                replayTimer.Stop();
                            }
                        }

                        trackBar.tryInvoke(() => trackBar.Value = lastMessageAdded);// (int)(totalReplayTime / 10000));
                        currentTimeLabel.tryInvoke(() =>
                            currentTimeLabel.Text = "Current Time: " +
                                ((double)(totalReplayTime / 10000)) / 1000 + "s");
                    });
                };
                replayTimer.Start();
            });
        }

        Tuple<long, string>[] readLog(string[] lines)
        {
            return lines.Select(line =>
            {
                string[] parts = line.Split(' ');
                if (parts.Length <= 1)
                    return null;
                if (!parts[0].EndsWith(":"))
                    return null;

                long time;
                if (!long.TryParse(parts[0].Substring(0, parts[0].Length - 1), out time))
                    return null;

                char[] chars = parts.Skip(1).Select(str =>
                {
                    if (str.Length != 2)
                        return '?';
                    if (((str[0] >= '0' && str[0] <= '9') || (str[0] >= 'A' && str[0] <= 'F')) &&
                        ((str[1] >= '0' && str[1] <= '9') || (str[1] >= 'A' && str[1] <= 'F')))
                    {
                        int high = (str[0] >= '0' && str[0] <= '9') ? (str[0] - '0') : (10 + str[0] - 'A');
                        int low = (str[1] >= '0' && str[1] <= '9') ? (str[1] - '0') : (10 + str[1] - 'A');
                        return (char)(16 * high + low);
                    }
                    else return '?';
                }).ToArray();

                return new Tuple<long, string>(time, new string(chars));
            }).ToArray();
        }

        long pauseStart;
        private void playButton_Click(object sender, EventArgs e)
        {
            replayTimer.Start();
            playButton.Enabled = false;
            pauseButton.Enabled = true;
            timeOfLastTick += DateTime.Now.ToFileTimeUtc() - pauseStart;
        }
        private void pauseButton_Click(object sender, EventArgs e)
        {
            replayTimer.Stop();
            playButton.Enabled = true;
            pauseButton.Enabled = false;
            pauseStart = DateTime.Now.ToFileTimeUtc();
        }

        private void speedUpButton_Click(object sender, EventArgs e)
        {
            speed *= 2;
            speedLabel.Text = "Speed: x" + speed;
        }

        private void speedDownButton_Click(object sender, EventArgs e)
        {
            speed /= 2;
            speedLabel.Text = "Speed: x" + speed;
        }

        private void ReplayForm_FormClosed(object sender, FormClosedEventArgs e)
        {
            replayTimer.Stop();
        }
    }
}
