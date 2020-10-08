/******************************************************************************
 *
 * 
 *
 *
 * Copyright (C) 1997-2015 by Dimitri van Heesch.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation under the terms of the GNU General Public License is hereby 
 * granted. No representations are made about the suitability of this software 
 * for any purpose. It is provided "as is" without express or implied warranty.
 * See the GNU General Public License for more details.
 *
 * Documents produced by Doxygen are derivative works derived from the
 * input used in their production; they are not affected by this license.
 *
 */

#include "rtfstyle.h"

#include <qfile.h>
#include <qtextstream.h>
#include <stdlib.h>

#include "message.h"


RTFListItemInfo rtf_listItemInfo[rtf_maxIndentLevels];

QCString rtf_title;
QCString rtf_subject;
QCString rtf_comments;
QCString rtf_company;
QCString rtf_logoFilename;
QCString rtf_author;
QCString rtf_manager;
QCString rtf_documentType;
QCString rtf_documentId;
QCString rtf_keywords;

char rtf_Style_Reset[] = "\\pard\\plain ";

#define RTF_LatexToc(lvl,nest,nxt,pos,twps)                                                                 \
                                                                                                            \
  { "LatexTOC"#lvl,                                                                                            \
    "\\s"#nest"\\li"#pos"\\sa"#twps"\\sb"#twps"\\widctlpar\\tqr\\tldot\\tx8640\\adjustright \\fs20\\cgrid ",\
    "\\sbasedon0 \\snext"#nxt" LatexTOC "#lvl                                                               \
  }

#define RTF_ListBullet(lvl,nest,nxt,pos,lvl2)                        \
  { "ListBullet"#lvl,                                                \
    "\\s"#nest"\\fi-360\\li"#pos"\\widctlpar\\jclisttab\\tx"#pos"{\\*\\pn \\pnlvlbody\\ilvl0\\ls"#lvl2"\\pnrnot0\\pndec }\\ls1\\adjustright \\fs20\\cgrid ", \
    "\\sbasedon0 \\snext"#nxt" \\sautoupd List Bullet "#lvl          \
  }

#define RTF_ListEnum(lvl,nest,nxt,pos)                       \
  { "ListEnum"#lvl,                                          \
    "\\s"#nest"\\fi-360\\li"#pos"\\widctlpar\\fs20\\cgrid ", \
    "\\sbasedon0 \\snext"#nxt" \\sautoupd List Enum "#lvl    \
  }

#define RTF_CodeExample(lvl,nest,nxt,pos)                                                    \
  { "CodeExample"#lvl,                                                                       \
    "\\s"#nest"\\li"#pos"\\widctlpar\\adjustright \\shading1000\\cbpat8 \\f2\\fs16\\cgrid ", \
    "\\sbasedon0 \\snext"#nxt" Code Example "#lvl                                            \
  }

#define RTF_ListContinue(lvl,nest,nxt,pos)                                              \
  { "ListContinue"#lvl,                                                                 \
    "\\s"#nest"\\li"#pos"\\sa60\\sb30\\qj\\widctlpar\\qj\\adjustright \\fs20\\cgrid ",  \
    "\\sbasedon0 \\snext"#nxt" List Continue "#lvl                                      \
  }

#define RTF_DescContinue(lvl,nest,nxt,pos)                                              \
  { "DescContinue"#lvl,                                                                 \
    "\\s"#nest"\\li"#pos"\\widctlpar\\ql\\adjustright \\fs20\\cgrid ",                  \
    "\\sbasedon0 \\snext"#nxt" DescContinue "#lvl                                       \
  }

Rtf_Style_Default rtf_Style_Default[] =
{
  { "Heading1",
    "\\s1\\sb240\\sa60\\keepn\\widctlpar\\adjustright \\b\\f1\\fs36\\kerning36\\cgrid ",
    "\\sbasedon0 \\snext0 heading 1"
  },
  { "Heading2",
    "\\s2\\sb240\\sa60\\keepn\\widctlpar\\adjustright \\b\\f1\\fs28\\kerning28\\cgrid ",
    "\\sbasedon0 \\snext0 heading 2"
  },
  { "Heading3",
    "\\s3\\sb240\\sa60\\keepn\\widctlpar\\adjustright \\b\\f1\\cgrid ",
    "\\sbasedon0 \\snext0 heading 3"
  },
  { "Heading4",
    "\\s4\\sb240\\sa60\\keepn\\widctlpar\\adjustright \\b\\f1\\fs20\\cgrid ",
    "\\sbasedon0 \\snext0 heading 4;}{\\*\\cs10 \\additive Default Paragraph Font"
  },
  { "Heading5",
    "\\s5\\sb90\\sa30\\keepn\\widctlpar\\adjustright \\b\\f1\\fs20\\cgrid ",
    "\\sbasedon0 \\snext0 heading 5;}{\\*\\cs10 \\additive Default Paragraph Font"
  },
  { "Title",
    "\\s15\\qc\\sb240\\sa60\\widctlpar\\outlinelevel0\\adjustright \\b\\f1\\fs32\\kerning28\\cgrid ",
    "\\sbasedon0 \\snext15 Title"
  },
  { "SubTitle",
    "\\s16\\qc\\sa60\\widctlpar\\outlinelevel1\\adjustright \\f1\\cgrid ",
    "\\sbasedon0 \\snext16 Subtitle"
  },
  { "BodyText",
    "\\s17\\sa60\\sb30\\widctlpar\\qj \\fs22\\cgrid ",
    "\\sbasedon0 \\snext17 BodyText"
  },
  { "DenseText",
    "\\s18\\widctlpar\\fs22\\cgrid ",
    "\\sbasedon0 \\snext18 DenseText"
  },
  { "Header",
    "\\s28\\widctlpar\\tqc\\tx4320\\tqr\\tx8640\\adjustright \\fs20\\cgrid ",
    "\\sbasedon0 \\snext28 header"
  },
  { "Footer",
    "\\s29\\widctlpar\\tqc\\tx4320\\tqr\\tx8640\\qr\\adjustright \\fs20\\cgrid ",
    "\\sbasedon0 \\snext29 footer"
  },
  { "GroupHeader",
    "\\s30\\li360\\sa60\\sb120\\keepn\\widctlpar\\adjustright \\b\\f1\\fs20\\cgrid ",
    "\\sbasedon0 \\snext30 GroupHeader"
  },

  RTF_CodeExample( 0, 40, 41,   0),
  RTF_CodeExample( 1, 41, 42, 360),
  RTF_CodeExample( 2, 42, 43, 720),
  RTF_CodeExample( 3, 43, 44,1080),
  RTF_CodeExample( 4, 44, 45,1440),
  RTF_CodeExample( 5, 45, 46,1800),
  RTF_CodeExample( 6, 46, 47,2160),
  RTF_CodeExample( 7, 47, 48,2520),
  RTF_CodeExample( 8, 48, 49,2880),
  RTF_CodeExample( 9, 49, 50,3240),
  RTF_CodeExample(10, 50, 51,3600),
  RTF_CodeExample(11, 51, 52,3960),
  RTF_CodeExample(12, 52, 53,4320),
  RTF_CodeExample(13, 53, 53,4680),

  RTF_ListContinue( 0, 60, 61,   0),
  RTF_ListContinue( 1, 61, 62, 360),
  RTF_ListContinue( 2, 62, 63, 720),
  RTF_ListContinue( 3, 63, 64,1080),
  RTF_ListContinue( 4, 64, 65,1440),
  RTF_ListContinue( 5, 65, 66,1800),
  RTF_ListContinue( 6, 66, 67,2160),
  RTF_ListContinue( 7, 67, 68,2520),
  RTF_ListContinue( 8, 68, 69,2880),
  RTF_ListContinue( 9, 69, 70,3240),
  RTF_ListContinue(10, 70, 71,3600),
  RTF_ListContinue(11, 71, 72,3960),
  RTF_ListContinue(12, 72, 73,4320),
  RTF_ListContinue(13, 73, 73,4680),

  RTF_DescContinue( 0, 80, 81,   0),
  RTF_DescContinue( 1, 81, 82, 360),
  RTF_DescContinue( 2, 82, 83, 720),
  RTF_DescContinue( 3, 83, 84,1080),
  RTF_DescContinue( 4, 84, 85,1440),
  RTF_DescContinue( 5, 85, 86,1800),
  RTF_DescContinue( 6, 86, 87,2160),
  RTF_DescContinue( 7, 87, 88,2520),
  RTF_DescContinue( 8, 88, 89,2880),
  RTF_DescContinue( 9, 89, 90,3240),
  RTF_DescContinue(10, 90, 91,3600),
  RTF_DescContinue(11, 91, 92,3960),
  RTF_DescContinue(12, 92, 93,4320),
  RTF_DescContinue(13, 93, 93,4680),

  RTF_LatexToc( 0,100,101,   0,30),
  RTF_LatexToc( 1,101,102, 360,27),
  RTF_LatexToc( 2,102,103, 720,24),
  RTF_LatexToc( 3,103,104,1080,21),
  RTF_LatexToc( 4,104,105,1440,18),
  RTF_LatexToc( 5,105,106,1800,15),
  RTF_LatexToc( 6,106,107,2160,12),
  RTF_LatexToc( 7,107,108,2520, 9),
  RTF_LatexToc( 8,108,109,2880, 6),
  RTF_LatexToc( 9,109,110,3240, 3),
  RTF_LatexToc(10,110,111,3600, 3),
  RTF_LatexToc(11,111,112,3960, 3),
  RTF_LatexToc(12,112,113,4320, 3),
  RTF_LatexToc(13,113,113,4680, 3),

  RTF_ListBullet( 0,120,121, 360, 1),
  RTF_ListBullet( 1,121,122, 720, 2),
  RTF_ListBullet( 2,122,123,1080, 3),
  RTF_ListBullet( 3,123,124,1440, 4),
  RTF_ListBullet( 4,124,125,1800, 5),
  RTF_ListBullet( 5,125,126,2160, 6),
  RTF_ListBullet( 6,126,127,2520, 7),
  RTF_ListBullet( 7,127,128,2880, 8),
  RTF_ListBullet( 8,128,129,3240, 9),
  RTF_ListBullet( 9,129,130,3600,10),
  RTF_ListBullet(10,130,131,3960,11),
  RTF_ListBullet(11,131,132,4320,12),
  RTF_ListBullet(12,132,133,4680,13),
  RTF_ListBullet(13,133,133,5040,14),

  RTF_ListEnum( 0,140,141, 360),
  RTF_ListEnum( 1,141,142, 720),
  RTF_ListEnum( 2,142,143,1080),
  RTF_ListEnum( 3,143,144,1440),
  RTF_ListEnum( 4,144,145,1800),
  RTF_ListEnum( 5,145,146,2160),
  RTF_ListEnum( 6,146,147,2520),
  RTF_ListEnum( 7,147,148,2880),
  RTF_ListEnum( 8,148,149,3240),
  RTF_ListEnum( 9,149,150,3600),
  RTF_ListEnum(10,150,151,3960),
  RTF_ListEnum(11,151,152,4320),
  RTF_ListEnum(12,152,153,4680),
  RTF_ListEnum(13,153,153,5040),

  { 0,
    0,
    0
  }
};

const QRegExp StyleData::s_clause("\\\\s[0-9]+\\s*");

StyleData::StyleData(const char* reference, const char* definition)
{
  int start = s_clause.match(reference); ASSERT(start >= 0);
  reference += start;
  index = (int)atol(reference + 2); ASSERT(index > 0);

  ASSERT(reference != 0);
  size_t size = 1 + strlen(reference);
  memcpy(this->reference = new char[size], reference, size);

  ASSERT(definition != 0);
  size = 1 + strlen(definition);
  memcpy(this->definition = new char[size], definition, size);
}

StyleData::~StyleData()
{
  delete[] reference;
  delete[] definition;
}

bool StyleData::setStyle(const char* s, const char* styleName)
{
  static const QRegExp subgroup("^{[^}]*}\\s*");
  static const QRegExp any_clause("^\\\\[a-z][a-z0-9-]*\\s*");

  int len = 0;     // length of a particular RTF formatting control
  int ref_len = 0; // length of the whole formatting section of a style
  int start = s_clause.match(s, 0, &len);
  if (start < 0)
  {
    err("Style sheet '%s' contains no '\\s' clause.\n{%s}\n", styleName, s);
    return FALSE;
  }
  s += start;
  index = (int)atol(s + 2); ASSERT(index > 0);

  // search for the end of pure formatting codes
  const char* end = s + len;
  ref_len = len;
  bool haveNewDefinition = TRUE;
  for(;;)
  {
    if (*end == '{')
    {
      // subgroups are used for \\additive
      if (0 != subgroup.match(end, 0, &len))
        break;
      else
      {
        end += len;
        ref_len += len;
      }
    }
    else if (*end == '\\')
    {
      if (0 == qstrncmp(end, "\\snext", 6))
        break;
      if (0 == qstrncmp(end, "\\sbasedon", 9))
        break;
      if (0 != any_clause.match(end, 0, &len))
        break;
      end += len;
      ref_len += len;
    }
    else if (*end == 0)
    { // no style-definition part, keep default value
      haveNewDefinition = FALSE;
      break;
    }
    else // plain name without leading \\snext
      break;
  }
  delete[] reference;
  reference = new char[ref_len + 1];
  memcpy(reference, s, ref_len); 
  reference[ref_len] = 0;
  if (haveNewDefinition)
  {
    delete[] definition;
    size_t size = 1 + strlen(end);
    definition = new char[size];
    memcpy(definition, end, size);
  }
  return TRUE;
}

void loadStylesheet(const char *name, QDict<StyleData>& dict)
{
  QFile file(name);
  if (!file.open(IO_ReadOnly))
  {
    err("Can't open RTF style sheet file %s. Using defaults.\n",name);
    return;
  }
  msg("Loading RTF style sheet %s...\n",name);

  static const QRegExp separator("[ \t]*=[ \t]*");
  uint lineNr=1;
  QTextStream t(&file);
  t.setEncoding(QTextStream::UnicodeUTF8);

  while (!t.eof())
  {
    QCString s(4096); // string buffer of max line length
    s = t.readLine().stripWhiteSpace().utf8();
    if (s.isEmpty() || s.at(0)=='#') continue; // skip blanks & comments
    int sepLength;
    int sepStart = separator.match(s,0,&sepLength);
    if (sepStart<=0) // no valid assignment statement
    {
      warn(name,lineNr,"Assignment of style sheet name expected!\n");
      continue;
    }
    QCString key=s.left(sepStart);
    if (dict[key]==0) // not a valid style sheet name
    {
      warn(name,lineNr,"Invalid style sheet name %s ignored.\n",key.data());
      continue;
    }
    StyleData* styleData = dict.find(key);
    if (styleData == 0)
    {
      warn(name,lineNr,"Unknown style sheet name %s ignored.\n",key.data());
      continue;
    }
    s+=" "; // add command separator
    styleData->setStyle(s.data() + sepStart + sepLength, key.data());
    lineNr++;
  }
}

QDict<StyleData> rtf_Style(257);

void loadExtensions(const char *name)
{
  QFile file(name);
  if (!file.open(IO_ReadOnly))
  {
    err("Can't open RTF extensions file %s. Using defaults.\n",name);
    return;
  }
  msg("Loading RTF extensions %s...\n",name);

  static const QRegExp separator("[ \t]*=[ \t]*");
  uint lineNr=1;
  QTextStream t(&file);
  t.setEncoding(QTextStream::UnicodeUTF8);

  while (!t.eof())
  {
    QCString s(4096); // string buffer of max line length
    s = t.readLine().stripWhiteSpace().utf8();
    if (s.length()==0 || s.at(0)=='#') continue; // skip blanks & comments
    int sepLength;
    int sepStart = separator.match(s,0,&sepLength);
    if (sepStart<=0) // no valid assignment statement
    {
      warn(name,lineNr,"Assignment of extension field expected!\n");
      continue;
    }
    QCString key=s.left(sepStart);
    QCString data=s.data() + sepStart + sepLength;

    if (key == "Title")           rtf_title            = data.data();
    if (key == "Subject")         rtf_subject          = data.data();
    if (key == "Comments")        rtf_comments         = data.data();
    if (key == "Company")         rtf_company          = data.data();
    if (key == "LogoFilename")    rtf_logoFilename     = data.data();
    if (key == "Author")          rtf_author           = data.data();
    if (key == "Manager")         rtf_manager          = data.data();
    if (key == "DocumentType")    rtf_documentType     = data.data();
    if (key == "DocumentId")      rtf_documentId       = data.data();
    if (key == "Keywords")        rtf_keywords         = data.data();
    lineNr++;
  }
}

