// Fetch PSX CD cover from Google images

// https://stackoverflow.com/questions/27846337/select-and-download-random-image-from-google

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using System.Net;
using System.IO;
using System.Text.RegularExpressions;

public class FetchCDCover
{

    /// <summary>
    /// Get CD Disk cover of specified game title
    /// </summary>
    /// <param name="title">Game name / Game ID from config</param>
    /// <returns></returns>
    public Image GetCover (string title)
    {
        Image image = null;

        title += " psx game cover front";

        List<string> topics = title.Split(' ').ToList();

        string html = GetHtmlCode(topics);
        List<string> urls = GetUrls(html);

        if ( urls.Count == 0)
        {
            return null;
        }

        var rnd = new Random();

        // I trust Google's deepq
        int randomUrl = 0;  //rnd.Next(0, Math.Min(7, urls.Count - 1));

        string luckyUrl = urls[randomUrl];

        byte[] imageRaw = GetImage(luckyUrl);
        using (var ms = new MemoryStream(imageRaw))
        {
            image = Image.FromStream(ms);
        }

        return image;
    }

    private string GetHtmlCode(List<string> topics)
    {
        var rnd = new Random();

        string query = String.Join("+", topics);

        string url = "https://www.google.com/search?q=" + query + "&tbm=isch";
        string data = "";

        var request = (HttpWebRequest)WebRequest.Create(url);
        request.Accept = "text/html, application/xhtml+xml, */*";
        request.UserAgent = "Mozilla/5.0 (Windows NT 6.1; WOW64; Trident/7.0; rv:11.0) like Gecko";

        var response = (HttpWebResponse)request.GetResponse();

        using (Stream dataStream = response.GetResponseStream())
        {
            if (dataStream == null)
                return "";
            using (var sr = new StreamReader(dataStream))
            {
                data = sr.ReadToEnd();
            }
        }
        return data;
    }

    private List<string> GetUrls(string html)
    {
        var urls = new List<string>();

        int ndx = html.IndexOf("\"ou\"", StringComparison.Ordinal);

        while (ndx >= 0)
        {
            ndx = html.IndexOf("\"", ndx + 4, StringComparison.Ordinal);
            ndx++;
            int ndx2 = html.IndexOf("\"", ndx, StringComparison.Ordinal);
            string url = html.Substring(ndx, ndx2 - ndx);
            urls.Add(url);
            ndx = html.IndexOf("\"ou\"", ndx2, StringComparison.Ordinal);
        }
        return urls;
    }

    private byte[] GetImage(string url)
    {
        var request = (HttpWebRequest)WebRequest.Create(url);
        var response = (HttpWebResponse)request.GetResponse();

        using (Stream dataStream = response.GetResponseStream())
        {
            if (dataStream == null)
                return null;
            using (var sr = new BinaryReader(dataStream))
            {
                byte[] bytes = sr.ReadBytes(100000000);

                return bytes;
            }
        }

        return null;
    }

}
