using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Tools.DotNETCommon;

namespace UnrealBuildTool
{
	/// <summary>
	/// Stores the state of a directory. May or may not exist.
	/// </summary>
	class DirectoryItem
	{
		/// <summary>
		/// Full path to the directory on disk
		/// </summary>
		public readonly DirectoryReference Location;

		/// <summary>
		/// Cached value for whether the directory exists
		/// </summary>
		DirectoryInfo Info;

		/// <summary>
		/// Cached map of name to subdirectory item
		/// </summary>
		List<DirectoryItem> Directories;

		/// <summary>
		/// Cached map of name to file
		/// </summary>
		List<FileItem> Files;

		/// <summary>
		/// Global map of location to item
		/// </summary>
		static ConcurrentDictionary<DirectoryReference, DirectoryItem> LocationToItem = new ConcurrentDictionary<DirectoryReference, DirectoryItem>();

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="Location">Path to this directory</param>
		/// <param name="Info">Information about this directory</param>
		private DirectoryItem(DirectoryReference Location, DirectoryInfo Info)
		{
			this.Location = Location;
			this.Info = Info;
		}

		/// <summary>
		/// The name of this directory
		/// </summary>
		public string Name
		{
			get { return Info.Name; }
		}

		/// <summary>
		/// Whether the directory exists or not
		/// </summary>
		public bool Exists
		{
			get { return Info.Exists; }
		}

		/// <summary>
		/// The last write time of the file.
		/// </summary>
		public DateTime LastWriteTimeUtc
		{
			get { return Info.LastWriteTimeUtc; }
		}

		/// <summary>
		/// Gets a new directory item by combining the existing directory item with the given path fragments
		/// </summary>
		/// <param name="BaseDirectory">Base directory to append path fragments to</param>
		/// <param name="Fragments">The path fragments to append</param>
		/// <returns>Directory item corresponding to the combined path</returns>
		public static DirectoryItem Combine(DirectoryItem BaseDirectory, params string[] Fragments)
		{
			return DirectoryItem.GetItemByDirectoryReference(DirectoryReference.Combine(BaseDirectory.Location, Fragments));
		}

		/// <summary>
		/// Finds or creates a directory item from its location
		/// </summary>
		/// <param name="Location">Path to the directory</param>
		/// <returns>The directory item for this location</returns>
		public static DirectoryItem GetItemByDirectoryReference(DirectoryReference Location)
		{
			DirectoryItem Result;
			if(!LocationToItem.TryGetValue(Location, out Result))
			{
				DirectoryItem NewItem = new DirectoryItem(Location, new DirectoryInfo(Location.FullName));
				if(LocationToItem.TryAdd(Location, NewItem))
				{
					Result = NewItem;
				}
				else
				{
					Result = LocationToItem[Location];
				}
			}
			return Result;
		}

		/// <summary>
		/// Finds or creates a directory item from a DirectoryInfo object
		/// </summary>
		/// <param name="Info">Path to the directory</param>
		/// <returns>The directory item for this location</returns>
		public static DirectoryItem GetItemByDirectoryInfo(DirectoryInfo Info)
		{
			DirectoryReference Location = new DirectoryReference(Info);

			DirectoryItem Result;
			if(!LocationToItem.TryGetValue(Location, out Result))
			{
				DirectoryItem NewItem = new DirectoryItem(Location, Info);
				if(LocationToItem.TryAdd(Location, NewItem))
				{
					Result = NewItem;
				}
				else
				{
					Result = LocationToItem[Location];
				}
			}
			return Result;
		}

		/// <summary>
		/// Reset the contents of the directory and allow them to be fetched again
		/// </summary>
		public void Refresh()
		{
			Directories = null;
			Files = null;
		}

		/// <summary>
		/// Caches the subdirectories of this directories
		/// </summary>
		public void CacheDirectories()
		{
			if(Directories == null)
			{
				List<DirectoryItem> NewDirectories = new List<DirectoryItem>();
				if(Info.Exists)
				{
					foreach(DirectoryInfo SubDirectoryInfo in Info.EnumerateDirectories())
					{
						if(SubDirectoryInfo.Name.Length == 1 && SubDirectoryInfo.Name[0] == '.')
						{
							continue;
						}
						else if(SubDirectoryInfo.Name.Length == 2 && SubDirectoryInfo.Name[0] == '.' && SubDirectoryInfo.Name[1] == '.')
						{
							continue;
						}
						else
						{
							NewDirectories.Add(DirectoryItem.GetItemByDirectoryInfo(SubDirectoryInfo));
						}
					}
				}
				Directories = NewDirectories;
			}
		}

		/// <summary>
		/// Enumerates all the subdirectories
		/// </summary>
		/// <returns>Sequence of subdirectory items</returns>
		public IEnumerable<DirectoryItem> EnumerateDirectories()
		{
			CacheDirectories();
			return Directories;
		}

		/// <summary>
		/// Caches the files in this directory
		/// </summary>
		public void CacheFiles()
		{
			if(Files == null)
			{
				List<FileItem> NewFiles = new List<FileItem>();
				if(Info.Exists)
				{
					foreach(FileInfo FileInfo in Info.EnumerateFiles())
					{
						NewFiles.Add(FileItem.GetItemByFileInfo(FileInfo));
					}
				}
				Files = NewFiles;
			}
		}

		/// <summary>
		/// Enumerates all the files
		/// </summary>
		/// <returns>Sequence of FileItems</returns>
		public IEnumerable<FileItem> EnumerateFiles()
		{
			CacheFiles();
			return Files;
		}

		/// <summary>
		/// Formats this object as a string for debugging
		/// </summary>
		/// <returns>Location of the directory</returns>
		public override string ToString()
		{
			return Location.FullName;
		}
	}
}
