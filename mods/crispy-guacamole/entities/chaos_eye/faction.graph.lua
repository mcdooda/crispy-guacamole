script {
  links = {
    {
      1,
      1,
      2,
      1,
    },
    {
      3,
      1,
      1,
      1,
    },
  },
  nodes = {
    {
      name = 'components/factioncomponent',
    },
    {
      loadArguments = {
        {
          5,
          'Table',
        },
      },
      name = 'script/scriptoutput',
    },
    {
      loadArguments = {
        'scourge',
      },
      name = 'string',
    },
  },
}