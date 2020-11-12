using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.Mvc;

namespace Editor_Rest_API.Controllers
{
    public class EditorController : Controller
    {
        // GET: Editor
        public ActionResult Index()
        {
            return View();
        }

        public string Get()
        {
            return "Test";
        }
    }
}